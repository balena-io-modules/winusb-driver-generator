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

const winusbDriverGenerator = require('.');

const VENDOR_ID = 0x0a5c;
const PRODUCT_ID = 0x2764;

if (winusbDriverGenerator.hasDriver(VENDOR_ID, PRODUCT_ID)) {
  console.log(`Device: ${VENDOR_ID}:${PRODUCT_ID} already has a driver, omitting`);
} else {
  console.log(`Device: ${VENDOR_ID}:${PRODUCT_ID} lacks a driver, installing`);
  winusbDriverGenerator.associate(VENDOR_ID, PRODUCT_ID, 'Raspberry Pi USB boot');
  console.log('Done');
}
