# This Incredible, Battery-Free LoRa ePaper Display Is "Flexing" on Your PCB Construction Techniques!
PaperiNode is a TTN-connected, energy-harvesting, Arduino-compatible, flexible E Ink display that does lots for a device without batteries.
https://www.hackster.io/news/this-incredible-battery-free-lora-epaper-display-is-flexing-on-your-pcb-construction-techniques-e855909f135a

Retrieved from hackster.io on July 28, 2020 by Remko Welling

![PaperiNode_image_1](images/hacksterio_01.jpg?raw=true "PaperiNode_image_1")

Once in a while, we are treated to a concrete vision of things and technologies to come.

There are a multitude of new technologies on the horizon, intended to show us the benefits of things like low power, wireless communications. Flexible electronics. Energy harvesting — indeed, the world looks like a very bright place if all of these technologies are put to good use.

While most of us have seen these technologies crop up in one form or another, we don't often see them all employed to good use at once. Not to dismiss any of them in any way — any product that properly embraces new technology will often be better for it. We're just not quite at the level of technological advancement that we need to be for these technologies to become common place in our designs.

Heck, the manufacturers themselves are only really just putting out development boards and kits for their own, respective pieces of the pie. We can get hold of energy harvesting kits from [XXX], or thin film batteries from the likes of ST, but they aren't exactly legacy products yet!

![PaperiNode_image_2](images/hacksterio_02.jpg?raw=true "PaperiNode_image_2")

So, it is with absolute pleasure that I find this incredible project in my inbox, from Robert Poser — [PaperiNode](https://github.com/RobPo/PaperiNode): a flexible, low power, energy-harvesting, LoRa-enabled, [electronic paper display!(https://github.com/RobPo/Paperino).

![PaperiNode_image_3](images/hacksterio_03.gif?raw=true "PaperiNode_image_3")

More than just a mouthful off suggestions for EE buzzword bingo, this device is a beautiful showcase of what's currently hot in emerging technologies, designed and engineered to fit together in a way that demonstrates the advantages of each part, in a functional and eye catching way.

![PaperiNode_image_4](images/hacksterio_04.jpg?raw=true "PaperiNode_image_4")

We don't often see this level of teamwork from the even individual companies who design and manufacture the component parts of this contraption, so to see a single enterprise manage to do so, in such a slick fashion is a real treat for us to see, and we're going to dive in to this design, to see if there aren't a few tricks and pointers that we can leverage in looking towards linking them into out own products!

At its core, **PaperiNode** is a slick technology demonstrator for [Paperino](https://github.com/RobPo/Paperino) — the ecosystem of flexible [EPD](https://en.wikipedia.org/wiki/Electronic_paper) (electrophoretic display, though more commonly Electronic Paper Display...) [products](https://www.plasticlogic.com/displays/) from [Plastic Logic](https://www.plasticlogic.com/). While there are other manufacturers who are producing similar EPD products, Plastic Logic specialize in ultra-flexible EPD parts, and PaperiNode showcases that in style!

We're always keen to see new technologies, and it's a even more fun when these products are able to help others connect — in a somewhat less literal sense of the word, it just so happens that while Poser is based out of the PL offices in Dresden, Germany, I sit writing this article some 100m away from the PL offices over here in Cambridge in the UK!

We've got to laugh at the unexpected ways in which technology is able to help connect people, we really do!

##Popping the hood on the PaperiNode!

As soon as you take a look at the inside face of [PaperiNode](https://www.tindie.com/products/robertposer/paperinode-solarpowered-e-paper-node-for-lorawan/), it becomes clear that this devices is far, far more than a simple flexible EPD breakout board!

![PaperiNode_image_5](images/hacksterio_05.jpg?raw=true "PaperiNode_image_5")

It is in fact an Arduino-compatible development board — complete with example code and [EPD drivers](https://github.com/RobPo/Paperino) — that is capable of running eternally, from the meager amount of power it is able to source from its two **IXYS [SLMD121H04L](https://ixapps.ixys.com/DataSheet/SLMD121H04L_Nov16.pdf)** solar panels, mounted on the rear of the device!

![PaperiNode_image_6](images/hacksterio_06.jpg?raw=true "PaperiNode_image_6")

Harvesting the solar energy from the two photovoltaic panels is managed by a dedicated E-Peas [AEM10941](https://e-peas.com/product/aem10941/) MPPT PMIC, which is also able to keep the balance topped up on a hefty 400mF [HA230F](https://cap-xx-store.com/products/cap-xx-supercapacitor-ha130f?variant=31136020485) EDLC supercapcitor from Cap XX!

This hefty storage device allows the device to keep some charge tucked away for a rainy day, when overcast skies might mean that the solar cells can't quite muster the peak Tx current required by the radio.

As if that wasn't already impressive enough on its own — there's more! This robust energy harvesting setup gives enough headroom for some radio capabilities: PaperiNode is a fully LoRa compliant wireless node, thanks to the inclusion of the ever popular [RFM95W](https://www.hoperf.com/modules/lora/RFM95.html) IC from Hoperf.

![PaperiNode_image_7](images/hacksterio_07.jpg?raw=true "PaperiNode_image_7")

The core functionality of the device is provided by the time-proven, family favorite that all of us have likely used at least once in our lives — the [ATmega328PB](https://www.microchip.com/wwwproducts/en/ATmega328PB). This is the same CPU as the well-known workhorse of the Arduino, sporting 32KB of flash / 2KB of SRAM, running at 16MHz, only packaged up into a smaller, SMD TQFP part instead of the PDIP that is instantly recognizable to all.

![PaperiNode_image_8](images/hacksterio_08.jpg?raw=true "PaperiNode_image_8")

While the ATmega does pretty well at low power, it does even better when it's turned fully off. The inclusion of an external RTC IC, the Microchip [MCP7940M](https://www.microchip.com/wwwproducts/en/MCP7940M) — and its ability to output a configurable interrupt signal via its MFP pin, means that it's easy to architect your application to use only the absolute minimum of power required.

![PaperiNode_image_9](images/hacksterio_09.jpg?raw=true "PaperiNode_image_9")

The EPD module needs no power to hold a static image, and with the ATmega sat idle in "deep sleep" mode, waiting for an interrupt from the RTC to spring it into life, there is virtually no current consumed — a mere **2.4 uA** between display updates!

![PaperiNode_image_10](images/hacksterio_10.jpg?raw=true "PaperiNode_image_10")

Speaking of EPD, we had best actually touch on the specs of the module! Sourced from Plastic Logic, this [flexible display module](https://www.plasticlogic.com/product/1-1-display/) weighs in with a very usable resolution of 148 x 70 pixels, and an active area of 1.1". Perfect for some very readable fonts and graphics.

![PaperiNode_image_11](images/hacksterio_11.jpg?raw=true "PaperiNode_image_11")

As with any graphical application however, memory can be a bit of a premium. Although the 32KB of flash inside the ATmega goes a long way for application code, it's not a huge amount when it comes to image data, or even a few font sets! While there are some tricks you can pull, like compressing your image, the extra clock cycles needed to run the decompression routines can cost you dearly in power!

![PaperiNode_image_12](images/hacksterio_12.jpg?raw=true "PaperiNode_image_12")

With that in mind, it's great to see a **Winbond** [W25X40Cl](https://www.winbond.com/hq/product/code-storage-flash-memory/serial-nor-flash/?__locale=en&partNo=W25X40CL) 4Mbit FLASH memory on the schematic! That's a good amount of space to hold uncompressed bitmaps, meaning you can spend less time sat in run mode,, churning through clock cycles crunching the decompression operations, instead, needing to do little more than shift a few raw bytes back and forth between tween the peripherals.

![PaperiNode_image_13](images/hacksterio_13.jpg?raw=true "PaperiNode_image_13")

A few other peripherals, such as a [DS2401](https://www.maximintegrated.com/en/products/ibutton-one-wire/memory-products/DS2401.html) are included. This part is a Silicon Serial Number IC, basically a hard-coded 48-bit serial number, that is guaranteed to be unique across every device. This is a neat way to save programming each device with a UUID, and takes care of providing a guaranteed unique identity for the purposes of device identification, something critical to large scale networked device co-existence!

There's a lot going on with this device beyond the standard Arduino reference schematic, and the feat of packing everything into this flexible FPC assembly can perhaps be underappreciated until you see one of these displays with something that provides a sense of scale...

![PaperiNode_image_14](images/hacksterio_14.jpg?raw=true "PaperiNode_image_14")

This TTN-connected node really is just tiny! Its size makes things all the more impressive when you now consider how big those two PV cells on the back actually are, and then consider the amount of power this device must actually consume in order to reliably function from this source.

The PaperiNode is available for purchase on [Tindie](https://www.tindie.com/products/robertposer/paperinode-solarpowered-e-paper-node-for-lorawan/), and the GitHub that supports the project can be found [here](https://github.com/RobPo/PaperiNode)!

For more developments in the area of flexible displays and to follow along with the future PaperiNode generations, be sure to give Poser a look on Twitter — [@Paperino_io](https://twitter.com/Paperino_io)!