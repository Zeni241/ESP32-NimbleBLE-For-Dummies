

# ESP32-NimbleBLE for Dummies!!
![nimble-esp](https://user-images.githubusercontent.com/13729574/200117670-21afcf63-758c-4038-8ccc-cb54e4df8b42.jpg)

Apache MyNewt NimBLE is a highly configurable and BT SIG qualifiable BLE stack. It has much smaller heap and flash requirements as compared to Bluedroid.

I had hard time understanding how to use it with esp32. The examples of Nimble BLE in esp-idf documentation are very hard to follow.

 I consulted many resources to clear my concept (I have given the links to resources at the end).Once there, I decided to write down this tutorial for my future reference. But I would be pleased if it could help any one. Everyone is free to use it, comment and suggest improvement in it.

I expect that you already have understanding of concept of GAP, GATT, services and characteristics. Here is a link where these concepts are explained very well. https://learn.adafruit.com/introduction-to-bluetooth-low-energy/introduction .

**So as they always say, lets get started!**  :sunny:



In this tutorial we will create one service and two characteristics of this service.
Client will be able read or write to the first characteristics. This characteristic is created only to show how to handle when client can read or write to same characteristic.

In second characteristic we will learn to notify the client.

Our first step will be to create UUIDs (for distinct identification) of service and characteristics. As we are going to create custom service and characteristics so the UUIDs should be 128 bits.  

Go to https://www.uuidgenerator.net and generate three 128 bit UUIDs in bulk, one  for service and two for two characteristics. UUIDs will be of format (8-4-4-4-12).

I have generated this UUID for service:  b2bbc642-46da-11ed-b878-0242ac120002 .


To be able to use these UUIDs in our code, we have to convert this HEX format UUID  by reversing the order. 

The above UUID will become:
 0x02, 0x00, 0x12, 0xac, 0x42, 0x02, 0x78, 0xb8, 0xed, 0x11, 0xda, 0x46, 0x42, 0xc6, 0xbb, 0xb2 . (quit a boring exercise.)

 As we are planning to have two characteristics for this service, we will create two more UUIDs.

 Characteristic 1 UUID: c9af9c76-46de-11ed-b878-0242ac120002

 0x02, 0x00, 0x12, 0xac, 0x42, 0x02, 0x78, 0xb8, 0xed, 0x11, 0xde, 0x46, 0x76, 0x9c, 0xaf, 0xc9


 Characteristic 2 UUID:94f85bd8-46e5-11ed-b878-0242ac120002

 0x02, 0x00, 0x12, 0xac, 0x42, 0x02, 0x78, 0xb8, 0xed, 0x11, 0xe5, 0x46, 0xd8, 0x5b, 0xf8, 0x94
 

The rest I have explained in form of comments in the code.

Forgive me for over verbose comments and naming convention, but my emphasis is the clear explanation of what is happening in the code.

I hope my this attempt will get you started in use of nimble BLE. Later you would be able to go further on your own, Thanks.

Bellow are some very useful links for working with Nimble BLE.

+ chrome://bluetooth-internals/#adapter 

+ https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32/api-reference/bluetooth/nimble/index.html?highlight=nimble

+ https://github.com/espressif/esp-idf/tree/master/examples/bluetooth/nimble/bleprph

+ https://github.com/espressif/esp-idf/tree/master/examples/bluetooth/nimble/blehr

+ https://developers.google.com/web/updates/2015/07/interact-with-ble-devices-on-the-web 

+ https://mynewt.apache.org/latest/network/index.html 

+ https://mynewt.apache.org/latest/tutorials/ble/bleprph/bleprph.html 

+ https://mynewt.apache.org/latest/network/ble_hs/ble_hs.html 

+ https://mynewt.apache.org/latest/network/ble_hs/ble_gatts.html 

+ https://mynewt.apache.org/latest/network/ble_sec.html 

+ https://mynewt.apache.org/latest/get_started/index.html 


