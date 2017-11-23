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

#include "generator.h"

static int generator_list_driverless_devices(struct wdi_device_info **result) {
  struct wdi_options_create_list options_list = { 0 };

  // Only enumerate devices without a driver
  options_list.list_all = FALSE;

  // Don't include USB hubs or Composite Parent devices
  options_list.list_hubs = FALSE;

  return wdi_create_list(result, &options_list);
}

NAN_METHOD(hasDriver) {
  if (!info[0]->IsNumber()) {
    return Nan::ThrowError("Product id must be a number");
  }

  if (!info[1]->IsNumber()) {
    return Nan::ThrowError("Vendor id must be a number");
  }

  const unsigned short vendor = info[0]->Uint32Value();
  const unsigned short product = info[1]->Uint32Value();

  std::cout << "Searching for device: " << std::hex << "0x" << vendor << ":0x" << product << std::endl;
  wdi_set_log_level(WDI_LOG_LEVEL_WARNING);

  bool found = false;
  int code = WDI_SUCCESS;
  struct wdi_device_info *device_list_node;

  code = generator_list_driverless_devices(&device_list_node);
  if (code == WDI_SUCCESS) {
    for (; device_list_node != NULL; device_list_node = device_list_node->next) {
      std::cout << "Found: " << std::hex
        << "0x" << device_list_node->vid
        << ":0x" << device_list_node->pid
        << std::endl;
      if (device_list_node->vid == vendor && device_list_node->pid == product) {
        found = true;
        break;
      }
    }

    code = wdi_destroy_list(device_list_node);
    if (code != WDI_SUCCESS) {
      if (code == WDI_ERROR_BUSY) {
        Nan::ThrowError("Can't destroy device list");
      } else {
        Nan::ThrowError("Unknown error");
      }

      return;
    }

  // This means the returned list is empty.
  // See https://github.com/pbatard/libwdi/wiki/Usage
  //
  // If the list of driverless devices is empty, then we
  // can assume every device has a driver, including the
  // one the user asked about.
  } else if (code == WDI_ERROR_NO_DEVICE) {
    std::cout << "No driverless device detected. Assuming device has a driver" << std::endl;
    found = true;
  } else {
    if (code == WDI_ERROR_RESOURCE) {
      Nan::ThrowError("Memory could not be allocated internally");
    } else if (code == WDI_ERROR_BUSY) {
      Nan::ThrowError("Another instance of this function call is already in process");
    } else {
      Nan::ThrowError("Unknown error");
    }

    return;
  }

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(found));
}

NAN_MODULE_INIT(GeneratorInit) {
  NAN_EXPORT(target, hasDriver);
}

NODE_MODULE(Generator, GeneratorInit)
