
# ASUS S4000VA Hackintosh 黑苹果相关文件
## 简介 
![截图](screenshot/S4000VA.jpg)
这是华硕灵耀S4000VA（华硕超薄笔记本)的黑苹果相关文件, 其他华硕笔记本如果cpu也是8代 8250U或者8550U 放心套用这个EFI就行。

:warning: :warning: :warning: 

本项目使用Opencore引导 仅完美支持macOS 11.x Big Sur正式版。
项目在macOS 12.x Monterey下蓝牙也能用 但本人无时间完整测试
需要引导10.14或10.15的旧版系统 请移步到[release](https://github.com/stonexing/Asus-S4000VA8550-Hackintosh/releases)列表里查找旧版EFI

有问题可以提issue，有时间我会尽可能的提供帮助。 
## 本机硬件
- CPU: intel i7-8550U
- 内存: 8GB板载
- 硬盘: 1. M.2 512G 镁光 SSD; 2. SATA 480GB 东芝 SSD
- 网卡/蓝牙：原始网卡无解，更换为BCM94352Z (此网卡为第三方卡，需要用文件夹中的指定config替换)
- 显卡：UHD620,无独显
## 哪些可以用？
- [x] CPU变频 （并不完美 还没找到原因 运行bug sur比较卡顿）
- [x] 显卡硬件加速(QE/CI)
- [x] 音频完美(自己动手仿冒附源码及教程) 插耳机自动切换 麦克风(需要加dsdt开机激活一次)
- [x] HDMI输出 4k输出（开机后再插入hdmi可以双屏输出 笔记本内屏+4k显示器双输出）
- [x] USB3.0 USB2.0
- [x] WIFI/蓝牙(需更换网卡BCM94352Z后完美,天线长度不够切记多买跟天线)
- [x] 电量显示正常
- [x] 触控板（基本完美，但无法媲美白苹果）
- [x] 睡眠和唤醒
- [x] 亮度调节(小太阳)
- [x] Fn 音量调节
- [x] Fn快捷键 (带亮度调节)
- [x] 亮度调节(小太阳)
## 等待修复的问题
- [] 目前macOS Big Sur下完美, macOS Monterey蓝牙待驱动（我用的bcm94352z）


## Asus Laptop 华硕笔记本 hactosh
同为华硕笔记本配置相似y，以下给出友情链接

| 机型名称                          | 发布地址                                                     | 教程地址                                                     | 备注                                                       |
| --------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ---------------------------------------------------------- |
| Asus S4100VN                       | [链接](https://github.com/loong1992/Asus_S4100VN8250U_Hackintosh) | [链接](https://github.com/loong1992/Asus_S4100VN8250U_Hackintosh/blob/master/README.md) |   S4000跟S4100基本一致我用它的EFI一样完美，另外他对无线网卡方案总结比较细致                   |
| Asus P8P67 PRO                   | [链接](https://github.com/rafaelmaeuer/Asus-P8P67Pro-Hackintosh)  | [链接](https://github.com/rafaelmaeuer/Asus-P8P67Pro-Hackintosh/blob/master/readme.md) |   此配置跟此笔记本无关，我台式机配置几乎一样 |
| Asus A411UF                         | [链接](https://github.com/faizauthar12/Asus_A411UF_Hackintosh) |                                                              |   S4000VA的主板型号为X411 配置接近                   |
| Asus ZenBook 系列               | [链接](https://github.com/hieplpvip/Asus-ZENBOOK-HACKINTOSH) | [链接](https://www.tonymacx86.com/threads/guide-Asus-zenbook-using-clover-uefi-hotpatch.257448/) | 华硕笔记本基本换汤不换药一套方案出了很多型号 你懂的  |



####

# ASUS S4000VA Hackintosh 黑苹果与Asus VivoBook S510UA相关文件通用（安装bigsur可以直接用他的 但是他目前没更新Monterey的支持）
| 机型名称                          | 发布地址                                                     | 教程地址                                                     | 备注                                                       |
| --------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ---------------------------------------------------------- |
| Asus S510UA                       | [链接](https://github.com/LeeBinder/Asus-Vivobook-S510UA-Hackintosh/) | [链接](https://github.com/LeeBinder/Asus-Vivobook-S510UA-Hackintosh/) |   2021年6月17日 经尝试直接使用S510 UA的EFI安装macOS Big Sur更为完美 但他不支持Monterey[链接](https://github.com/LeeBinder/Asus-Vivobook-S510UA-Hackintosh/)                  |

============================================

