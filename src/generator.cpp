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

  std::cout << "Generating driver for device: " << std::hex << "0x" << vendor << ":0x" << product << std::endl;
  wdi_set_log_level(WDI_LOG_LEVEL_WARNING);

  bool found = false;
  struct wdi_device_info *device_list_node;

  if (generator_list_driverless_devices(&device_list_node) == WDI_SUCCESS) {
    for (; device_list_node != NULL; device_list_node = device_list_node->next) {
      printf("Found: V:%hu P:%hu\n", device_list_node->vid, device_list_node->pid);
      if (device_list_node->vid == vendor && device_list_node->pid == product) {
        found = true;
        break;
      }
    }
  }

  if (found) {
    std::cout << "Device has driver!" << std::endl;
  } else {
    std::cout << "Device has no driver!" << std::endl;
  }

  info.GetReturnValue().SetUndefined();
}

NAN_MODULE_INIT(GeneratorInit) {
  NAN_EXPORT(target, hasDriver);
}

NODE_MODULE(Generator, GeneratorInit)
