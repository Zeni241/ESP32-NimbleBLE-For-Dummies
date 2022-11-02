# ESP32-NimbleBLE
# NimbleBLE for Dummies!!
Apache MyNewt NimBLE is a highly configurable and BT SIG qualifiable BLE stack. It has much smaller heap and flash requirements as compared to Bluedroid.

I had hard time understanding its implementation in esp-idf examples given by espressif. So I decided to write down this tutorial for my future reference. 

I would be pleased if it could help any one. Everyone is free to comment and suggest improvement in it.

I have combined two examples in esp-idf documentation, “blephr” and “blehr”. I have also modified these examples to some extend for clear understanding.

I expect that you already have understanding of concept of GAP, GATT, services and characteristics. Here is a link where these concepts are explained very well. https://learn.adafruit.com/introduction-to-bluetooth-low-energy/introduction .

So as they always say, lets get started!

In this tutorial we will create one service and two characteristics of this service.
Client will be able read or write to the first characteristics. This characteristic is created only to show how to handle when client read or write to same characteristic.

In second characteristic we will learn to notify the client.

For this first we will create UUIDs (for distinct identification) for service and characteristics. As we are going to create custom service and characteristics so the UUIDs will be 128 bits.  Go to https://www.uuidgenerator.net/ and generate a 128 bit three UUIDs for service and two characterstic. UUID will be of format (8-4-4-4-12).
I have generated this UUID for service:  b2bbc642-46da-11ed-b878-0242ac120002 .


To use these UUIDs in our code, we have to convert this HEX format UUID  by reversing the order. The above UUID will become:
 0x02, 0x00, 0x12, 0xac, 0x42, 0x02, 0x78, 0xb8, 0xed, 0x11, 0xda, 0x46, 0x42, 0xc6, 0xbb, 0xb2 . (quit a boring exercise.)

 As we are planning to have two characterstics for this service, we will create two more UUIDs.

 Charaterstic 1 UUID: c9af9c76-46de-11ed-b878-0242ac120002

 0x02, 0x00, 0x12, 0xac, 0x42, 0x02, 0x78, 0xb8, 0xed, 0x11, 0xde, 0x46, 0x76, 0x9c, 0xaf, 0xc9


 Charaterstic 2 UUID:94f85bd8-46e5-11ed-b878-0242ac120002

 0x02, 0x00, 0x12, 0xac, 0x42, 0x02, 0x78, 0xb8, 0xed, 0x11, 0xe5, 0x46, 0xd8, 0x5b, 0xf8, 0x94
