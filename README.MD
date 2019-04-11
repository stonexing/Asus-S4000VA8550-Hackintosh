# ASUS S4000VA Hackintosh 黑苹果相关文件
## 简介 
![截图](screenshot/about.png)
这是华硕灵耀S4000VA（华硕超薄笔记本)的黑苹果相关文件, 这套配置我仅在苹果10.14(Macos Mojave)上测试完美 本人自己笔记本每个细节都查阅很多资料进行优化 完美度较高。

:warning: :warning: :warning: 

本项目提供的EFI并不能通用于所有的华硕笔记本，直接覆盖EFI分区可能导致无法启动等问题。推荐了解clover和黑苹果驱动的相关知识后，参考config.plist和驱动修改自己的EFI文件，切勿直接盲目使用。

有问题可以提issue，有时间我会尽可能的提供帮助。 
## 本机硬件
- CPU: intel i7-8550U
- 内存: 8GB板载
- 硬盘: 1. M.2 512G 镁光 SSD; 2. SATA 480GB 东芝 SSD
- 网卡/蓝牙：原始网卡无解，更换为BCM94352Z
- 显卡：HD620,无独显
## 哪些可以用？
- [x] CPU变频
- [x] 显卡硬件加速(QE/CI)
- [x] 亮度调节(小太阳)
- [x] Fn + F5,F6快捷键(小太阳) 自己动手编译AsusFnKeys
- [x] 音频完美(自己动手仿冒附源码及教程) 插耳机自动切换 麦克风完美
- [x] HDMI输出
- [x] USB3.0 USB2.0
- [x] WIFI/蓝牙(需更换网卡BCM94352Z后完美,天线长度不够切记多买跟天线)
- [x] 电量显示正常
- [x] 触控板（基本完美，但无法媲美白苹果）
- [x] 睡眠和唤醒
- [x] Fn快捷键 (带亮度调节)
## 等待修复的问题
- [ ] 快捷键Print sc这个键打印屏幕(能用但不完美 有兴趣自己编译修改我项目中的AsusFnKeys)

## 相关驱动
黑苹果使用的开源驱动的地址和使用说明记录，方便以后的升级


| kext  | 功能 | 
| ---------- | -----------|
| [FakePCIID*](https://github.com/RehabMan/OS-X-Fake-PCI-ID)  |包括显卡，WIFI和USB等多种特性|
|[FakeSMC](https://github.com/RehabMan/OS-X-FakeSMC-kozlek)|FakeSMC is an open source SMC device driver/emulator developed by netkas.|
|[ACPIBatteryManager](https://github.com/RehabMan/OS-X-ACPI-Battery-Driver)|电量显示相关|
|[Lilu](https://github.com/vit9696/Lilu)|部分kext依赖的框架|
|[AppleALC](https://github.com/vit9696/AppleALC)|声卡驱动|
|[WhateverGreen.kext](https://github.com/acidanthera/WhateverGreen)|Various patches necessary for certain ATI/AMD/Intel/Nvidia GPUs|
|||

####

