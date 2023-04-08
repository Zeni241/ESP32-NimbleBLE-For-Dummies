

# ESP32-NimbleBLE for Dummies!!
![nimble-esp](https://user-images.githubusercontent.com/13729574/200117670-21afcf63-758c-4038-8ccc-cb54e4df8b42.jpg)

Apache MyNewt NimBLE is a highly configurable and BT SIG qualifiable BLE stack. It has much smaller heap and flash requirements as compared to Bluedroid.

Due to these reasons I was very much interested to use it instead of Bluedroid or Bluetooth Classic for my esp32 devices. But unfortunately the examples of Nimble BLE in esp-idf documentation were of little help.

 So I consulted many other resources to clear my concept (I have given the links to resources at the end).Once there, **Nimble BLE turned out to be much simpler and straight forward compared to bluedroid or bluetooth classic**. So I decided to write down this tutorial for my future reference. I would be pleased if it could help any one. Everyone is free to use it, comment and suggest improvement in it.


**So as the cliche goes, lets get started!**  :sunny:



I expect that you already have understanding of concept of GAP, GATT, services and characteristics etc. . Here is a link where these concepts are explained very well. https://learn.adafruit.com/introduction-to-bluetooth-low-energy/introduction .


 We will create one service and one characteristics of this service. 

Client (your application) will be able read the characteristic value, write to it. Further this characteristic can send notifications to the client. 

- Our first step will be to create UUIDs (for distinct identification) of service and characteristic. As we are going to create custom service and characteristic so the UUIDs should be 128 bits.  
Go to https://www.uuidgenerator.net and generate two 128 bit UUIDs in bulk, one  for service and one for characteristic. UUIDs will be of format (8-4-4-4-12).

    For exaample I have generated this UUID for service:  

    *b2bbc642-46da-11ed-b878-0242ac120002* .

    To be able to use these UUIDs in our code, we have to convert this HEX format UUID  by reversing the order. 

    The above UUID will become:

    *0x02, 0x00, 0x12, 0xac, 0x42, 0x02, 0x78, 0xb8, 0xed, 0x11, 0xda, 0x46, 0x42, 0xc6, 0xbb, 0xb2 . (quit a boring exercise.)*

     Generated characteristic UUID: 

    *c9af9c76-46de-11ed-b878-0242ac120002*

    After conversion:

    *0x02, 0x00, 0x12, 0xac, 0x42, 0x02, 0x78, 0xb8, 0xed, 0x11, 0xde, 0x46, 0x76, 0x9c, 0xaf, 0xc9*


- The rest I have tried to explained in form of comments in the code in file nimble.c, because in my opinion that would be easier to follow instead of explaining it here.

- Once you have flashed this code to your module,   
    - open this link (chrome://bluetooth-internals/#adapter) in your browser.
    - Press  button "Devices" on the left.
    - Press "StartScan" on top right.
    - Find your device "nimble-ble" in the list and press "Inspect" on right. You will see Services TABs list.
    - Expand the service tab of your service UUID.
    - Expand its characterstic.
    - Select UTF-8 in dropdown list at right of value box.
    -Press read.You will get the value of your characterstic.
    -Write something in value box and press "Write" and you will see that value in your terminal. 
    - Unfortunately you cannont see notifications sent by your esp32 in this. For this you will have to write some code in your app.



Forgive me for over verbose comments and naming convention, but my emphasis is the clear explanation of what is happening in the code.

I hope my this attempt will get you started in using nimble BLE. Later you would be able to go further on your own, Thanks.


<span style="color:blue">**$\textcolor{magenta}{\textsf{And finally, if you find this repo useful, a star by you would be big encouragement for me}** </span>:wink: 

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


