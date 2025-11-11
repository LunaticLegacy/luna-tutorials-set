# Linux 新手使用说明书

## 0. 引言
Github: 月と猫 - LunaNeko https://github.com/LunaticLegacy

CC-BY-NC-SA 4.0

对于一个纯新手而言，个人推荐配置如下：
- 使用的系统：`Ubuntu`
- 文本编辑器；`Nano`

### 0.1 我要怎么看？

一般而言，在linux发行版中，命令行内会显示这个

```bash
用户名@设备名:当前工作目录$ 指令
```

例：
```bash
luna@LAPTOP-O7KSPF9L:/mnt/d/教程/luna-tutorials-set$ cd ~
luna@LAPTOP-O7KSPF9L:~$ ls
OpenCloudOS-Kernel  buildroot  dnf     git-server      orangepi-build  u-boot-orangepi
autobuild_oc        codes      docker  linux-orangepi  rv-sp-test-mod
luna@LAPTOP-O7KSPF9L:~$ cd ..
luna@LAPTOP-O7KSPF9L:/home$ ls
luna
luna@LAPTOP-O7KSPF9L:/home$
```

## 1. 基础命令

### 1.1 文件索引类

Linux的文件索引从**根目录**，即`/`开始。文件索引没有传统的分盘。

`ls` List files
- 展示当前工作目录下的文件夹。

例：
```bash
# 单纯显示本目录下有什么。
luna@LAPTOP-O7KSPF9L:~$ ls
OpenCloudOS-Kernel  buildroot  dnf     git-server      orangepi-build  u-boot-orangepi
autobuild_oc        codes      docker  linux-orangepi  rv-sp-test-mod
```

- 使用`ls -l`以描述文件详细信息。

```bash
# 显示目录下有什么，并显示其详细信息。
luna@LAPTOP-O7KSPF9L:~$ ls -l
total 44
drwxr-xr-x 28 luna luna 4096 Nov  1 20:45 OpenCloudOS-Kernel
drwxr-xr-x  7 luna luna 4096 Oct 22 13:36 autobuild_oc
drwxr-xr-x 19 luna luna 4096 Jul 15 11:21 buildroot
drwxr-xr-x  3 luna luna 4096 Oct 11 12:09 codes
drwxr-xr-x  4 luna luna 4096 Jul 14 20:39 dnf
drwxr-xr-x  3 luna luna 4096 Sep 29 15:43 docker
drwxr-xr-x  3 root root 4096 Sep 25 16:00 git-server
drwxr-xr-x 27 luna luna 4096 Nov  1 20:46 linux-orangepi
drwxr-xr-x  8 luna luna 4096 Oct 29 18:50 orangepi-build
drwxr-xr-x 14 luna luna 4096 Sep 25 00:00 rv-sp-test-mod
drwxr-xr-x 24 luna luna 4096 Oct 30 12:48 u-boot-orangepi
```

其中，`ls -l`的返回值格式为：`文件类型 硬链接数 所有者 所属组 文件大小 修改日期和时间 文件名`
- 文件类型：第一位为类型。`-`：普通文件，`d`：文件夹（Directory）
  - 字符1：
  - 剩余的字符，每3个一组。每组分别为：`rwx`。其中有`r`代表可读，有`w`代表可写，有`x`代表可执行。
  - 第一组：文件所有者的权限，第二组：同组用户的权限，第三组：其他用户的权限。
  - 权限的变动见[chmod]()，组的变动见
- 硬链接数：对目录，为“本目录下有多少个子目录”。
- 所有者、所属组：见[权限组]()。
- 文件大小：以字节为单位。
- 修改时间：例如`Oct 30 12:48`。对今年的文件通常显示时间，过去年份的会显示年份。
- 文件名：字面一i。


`cd` Change Directory
- 切换当前工作目录。
- `..`：上一级目录；`-`：上一次使用`cd`切换到的目标目录；`~`：当前用户主目录。

例：
```

```


### 1.x 查表

| 命令 | 速记 | 命令作用 | 参数长度 | 使用例 | 用例解释 |
| --- | --- | -------- | -------- | -------- | -------- |
| `cd` | Change Directory | 切换当前工作目录 | 1 | `cd ./lunamoon` | 切换到文件夹`lunamoon` |
