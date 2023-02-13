/*
 * Copyright 2017 resin.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "src/generator.h"
#include <napi.h>

static int
generator_list_driverless_devices(struct wdi_device_info **result) {
  struct wdi_options_create_list options_list = { 0 };

  // Only enumerate devices without a driver
  options_list.list_all = FALSE;

  // Don't include USB hubs or Composite Parent devices
  options_list.list_hubs = FALSE;

  return wdi_create_list(result, &options_list);
}

static int
generator_generate_winusb_inf(struct wdi_device_info *device,
                              const char * const name,
                              const char * const path) {
  struct wdi_options_prepare_driver options_driver = { 0 };
  // Use WinUSB for the driver
  options_driver.driver_type = WDI_WINUSB;
  return wdi_prepare_driver(device, path, name, &options_driver);
}

static int
generator_install_winusb_inf(struct wdi_device_info *device,
                             const char * const name,
                             const char * const path) {
  return wdi_install_driver(device, path, name, NULL);
}

Napi::Value ListDriverlessDevices(const Napi::CallbackInfo& info) {
  int code = WDI_SUCCESS;
  struct wdi_device_info *device_list_node;
  Napi::Env env = info.Env();
  Napi::Object devices = Napi::Array::New(env);
  wdi_set_log_level(WDI_LOG_LEVEL_WARNING);

  code = generator_list_driverless_devices(&device_list_node);
  if (code == WDI_SUCCESS) {
    uint32_t index = 0;
    for (; device_list_node != NULL
         ; device_list_node = device_list_node->next) {
      Napi::Object device = Napi::Object::New(env);
      device.Set(Napi::String::New(env, "vid"),
        Napi::Number::New(env, static_cast<double>(device_list_node->vid)));
      device.Set(Napi::String::New(env, "pid"),
        Napi::Number::New(env, static_cast<double>(device_list_node->pid)));
      device.Set(Napi::String::New(env, "hid"),
        Napi::String::New(env, device_list_node->hardware_id));
      device.Set(Napi::String::New(env, "did"),
        Napi::String::New(env, device_list_node->device_id));
      device.Set(index, device);
      index++;
    }

    code = wdi_destroy_list(device_list_node);
    if (code != WDI_SUCCESS) {
      Napi::TypeError::New(env, wdi_strerror(code))
        .ThrowAsJavaScriptException();
      return env.Null();
    }

  // This means the returned list is empty.
  // See https://github.com/pbatard/libwdi/wiki/Usage
  //
  // If the list of driverless devices is empty, then we
  // can assume every device has a driver.
  } else if (code != WDI_ERROR_NO_DEVICE) {
    Napi::TypeError::New(env, wdi_strerror(code))
      .ThrowAsJavaScriptException();
    return env.Null();
  }

  return devices;
}

Napi::Value Associate(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 3) {
    Napi::TypeError::New(env, "This function expects 3 arguments")
      .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Product id must be a number")
      .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[1].IsNumber()) {
    Napi::TypeError::New(env, "Vendor id must be a number")
      .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[2].IsString()) {
    Napi::TypeError::New(env, "Description must be a string")
      .ThrowAsJavaScriptException();
    return env.Null();
  }

  const uint32_t vendor = info[0].As<Napi::Number>().Uint32Value();
  const uint32_t product = info[1].As<Napi::Number>().Uint32Value();

  std::string description_string = info[2].As<Napi::String>().Utf8Value();
  char *description = new char[description_string.length() + 1];
  // cpplint suggests snprintf over strcpy, but the former
  // is not implemented in Visual Studio 2013.
  strcpy(description, description_string.c_str());  // NOLINT(runtime/printf)

  struct wdi_device_info *device_list_node;
  struct wdi_device_info device = {
    NULL,
    vendor,
    product,
    FALSE,
    0,
    description,
    NULL,
    NULL,
    NULL
  };

  bool matching_device_found = false;
  int code = WDI_SUCCESS;
  const char * const INF_NAME = "usb_device.inf";
  const char * const INF_PATH = "usb_driver";

  wdi_set_log_level(WDI_LOG_LEVEL_WARNING);

  std::cout << "Extracting driver files" << std::endl;
  code = generator_generate_winusb_inf(&device, INF_NAME, INF_PATH);
  if (code != WDI_SUCCESS) {
    Napi::TypeError::New(env, wdi_strerror(code))
      .ThrowAsJavaScriptException();
    return env.Null();
  }

  std::cout << "Installing driver" << std::endl;
  code = generator_list_driverless_devices(&device_list_node);
  if (code == WDI_SUCCESS) {
    for (; device_list_node != NULL
         ; device_list_node = device_list_node->next) {
      if (device_list_node->vid == vendor && device_list_node->pid == product) {
        device.hardware_id = device_list_node->hardware_id;
        device.device_id = device_list_node->device_id;
        matching_device_found = true;
        break;
      }
    }
  } else if (code == WDI_ERROR_NO_DEVICE) {
    matching_device_found = false;
  } else {
    Napi::TypeError::New(env, wdi_strerror(code))
      .ThrowAsJavaScriptException();
    return env.Null();
  }

  code = generator_install_winusb_inf(&device, INF_NAME, INF_PATH);
  if (code != WDI_SUCCESS) {
    Napi::TypeError::New(env, wdi_strerror(code))
      .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (matching_device_found) {
    code = wdi_destroy_list(device_list_node);
    if (code != WDI_SUCCESS) {
      Napi::TypeError::New(env, wdi_strerror(code))
        .ThrowAsJavaScriptException();
    return env.Null();
    }
  }

  delete [] description;
  return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(
    Napi::String::New(env, "listDriverlessDevices"),
    Napi::Function::New(env, ListDriverlessDevices));
  exports.Set(
    Napi::String::New(env, "associate"),
    Napi::Function::New(env, Associate));
  return exports;
}

NODE_API_MODULE(Generator, Init)
