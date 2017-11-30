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

NAN_METHOD(hasDriver) {
  if (info.Length() != 2) {
    return Nan::ThrowError("This function expects 2 arguments");
  }

  if (!info[0]->IsNumber()) {
    return Nan::ThrowError("Product id must be a number");
  }

  if (!info[1]->IsNumber()) {
    return Nan::ThrowError("Vendor id must be a number");
  }

  const uint16_t vendor = info[0]->Uint32Value();
  const uint16_t product = info[1]->Uint32Value();

  std::cout << "Searching for device: " << std::hex
    << "0x" << vendor
    << ":0x" << product
    << std::endl;
  wdi_set_log_level(WDI_LOG_LEVEL_WARNING);

  bool found = true;
  int code = WDI_SUCCESS;
  struct wdi_device_info *device_list_node;

  code = generator_list_driverless_devices(&device_list_node);
  if (code == WDI_SUCCESS) {
    for (; device_list_node != NULL
         ; device_list_node = device_list_node->next) {
      std::cout << "Found: " << std::hex
        << "0x" << device_list_node->vid
        << ":0x" << device_list_node->pid
        << std::endl;
      if (device_list_node->vid == vendor && device_list_node->pid == product) {
        found = false;
        break;
      }
    }

    code = wdi_destroy_list(device_list_node);
    if (code != WDI_SUCCESS) {
      return Nan::ThrowError(wdi_strerror(code));
    }

  // This means the returned list is empty.
  // See https://github.com/pbatard/libwdi/wiki/Usage
  //
  // If the list of driverless devices is empty, then we
  // can assume every device has a driver, including the
  // one the user asked about.
  } else if (code == WDI_ERROR_NO_DEVICE) {
    std::cout << "No driverless device detected. "
                 "Assuming device has a driver" << std::endl;
    found = true;
  } else {
    return Nan::ThrowError(wdi_strerror(code));
  }

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(found));
}

NAN_METHOD(associate) {
  if (info.Length() != 3) {
    return Nan::ThrowError("This function expects 3 arguments");
  }

  if (!info[0]->IsNumber()) {
    return Nan::ThrowError("Product id must be a number");
  }

  if (!info[1]->IsNumber()) {
    return Nan::ThrowError("Vendor id must be a number");
  }

  if (!info[2]->IsString()) {
    return Nan::ThrowError("Description must be a string");
  }

  const uint16_t vendor = info[0]->Uint32Value();
  const uint16_t product = info[1]->Uint32Value();

  // TODO(jviotti): Is there a better way to go from v8::String to char *?
  v8::String::Utf8Value description_v8(info[2]->ToString());
  std::string description_string = std::string(*description_v8);
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
    return Nan::ThrowError(wdi_strerror(code));
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
    return Nan::ThrowError(wdi_strerror(code));
  }

  code = generator_install_winusb_inf(&device, INF_NAME, INF_PATH);
  if (code != WDI_SUCCESS) {
    return Nan::ThrowError(wdi_strerror(code));
  }

  if (matching_device_found) {
    code = wdi_destroy_list(device_list_node);
    if (code != WDI_SUCCESS) {
      return Nan::ThrowError(wdi_strerror(code));
    }
  }

  delete [] description;
  info.GetReturnValue().SetUndefined();
}

NAN_MODULE_INIT(GeneratorInit) {
  NAN_EXPORT(target, hasDriver);
  NAN_EXPORT(target, associate);
}

NODE_MODULE(Generator, GeneratorInit)
