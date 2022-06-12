# DiaKEM Glucose LED Monitor

LED Panel Glucose Monitor

## Description

This project allows to display CGM-Data provided by DiaKEM on a simple LED Panel. The data will be continiously retrieved and prepared to show:

### Current glucose value

<a href="https://ibb.co/7JDZZXZ"><img src="https://i.ibb.co/TBG77k7/20220612-011908.jpg" alt="20220612-011908" border="0"></a>

### Difference to previous measurement

<a href="https://ibb.co/GJ1F32h"><img src="https://i.ibb.co/fSKQGpc/20220612-011917.jpg" alt="20220612-011917" border="0"></a>

### Elapsed time

<a href="https://ibb.co/NmhN8X1"><img src="https://i.ibb.co/wwHQ9TJ/20220612-011919.jpg" alt="20220612-011919" border="0"></a>

The LED Panel also highlights dangerous parts by colors:

* high / low glucose value
* fast  increase / decrease
* old data

## Use Case

Most people are using CGM-Sensors to continiously monitor their glucose value and also share this with other friends or family members. This data will
be provided over a specific application on the smartphone. Especially parents of children with typ-1 diabetes are forced to permanently check their
smartphone because of this. This was the main reason for me to create this solution to be able to check the current glucose value without having to accessmy smartphone 24 hours a day.

## Requirements

* DiaKEM Backend
* CGM-Bridge

## Setup

### Hardware

You will need the following hardware:

* Arduino compatible board like [ESP32](https://www.alibaba.com/product-detail/Wholesale-ESP32-ESP32S-ESP32-DEVKIT-V1_1600135879207.html?spm=a2700.galleryofferlist.normal_offer.d_title.75ee5a8fznPakc&s=p)
* [32x8 LED-Panel](https://www.alibaba.com/product-detail/Bendable-Pixel-Matrix-Programmable-RGB-SMD_62535391592.html?spm=a2700.galleryofferlist.normal_offer.d_image.1d4b781cYiWWla<F10>)

#### Wiring

<a href="https://ibb.co/kQGHZYL"><img src="https://i.ibb.co/Ln0kK4T/wiring-2.jpg" alt="wiring-2" border="0"></a>
<a href="https://ibb.co/NFgvh0V"><img src="https://i.ibb.co/7Sxfc3Y/wiring-1.jpg" alt="wiring-1" border="0"></a>

### Software

## Build and Deploy

This project is based on [PlatformIO](https://platformio.org/). The easist way to run PlatformIO is in combination with VSCode.
For a detailed tutorial you can check [this](https://docs.platformio.org/en/latest/integration/ide/vscode.html#ide-vscode).

After installing PlatformIO please clone this repository:

```bash
$ git clone ....
```

Now open VSCode and open the project with PlatformIO:

<a href="https://ibb.co/XLy06Nh"><img src="https://i.ibb.co/B34WvdX/Bildschirmfoto-2022-06-12-um-01-09-54.png" alt="Bildschirmfoto-2022-06-12-um-01-09-54" border="0"></a>

Now you are ready to set all necessary configuration parameter:

```
const char *ssid = "MY-WLAN-SSID";
const char *password = "Pa$$w0rd!";

const char *serverName = "https://my-diakem-instance/api/v1/graphql";
String appToken = "MY_APP_TOKEN";
```

We are ready to build and transfer the application to our ESP32 or other Arduino based board:

<a href="https://ibb.co/hHsw5ZS"><img src="https://i.ibb.co/92nSXV7/Bildschirmfoto-2022-06-12-um-01-13-42.png" alt="Bildschirmfoto-2022-06-12-um-01-13-42" border="0"></a>

## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. 
Any contributions you make are greatly appreciated.

If you have a suggestion that would make this better, please fork the repo and create a pull request. 
You can also simply open an issue with the tag "enhancement". Don't forget to give the project a star! Thanks again!

* Fork the Project
* Create your Feature Branch (git checkout -b feature/AmazingFeature)
* Commit your Changes (git commit -m 'Add some AmazingFeature')
* Push to the Branch (git push origin feature/AmazingFeature)
* Open a Pull Request

## License

Distributed under the MIT License.

## Contact

Selcuk Kekec

E-mail: [khskekec@gmail.com](khskekec@gmail.com)
