## 0. 引言
Github: 月と猫 - LunaNeko https://github.com/LunaticLegacy

CC-BY-NC-SA 4.0

本教程为自行总结，可能存在错误。如有错误问题请通过Repo反馈。

### 0.1 什么是Docker？

Docker的基础运行机制可分为3个部分：
- 镜像：一个只读的模板，创建容器时使用镜像进行创建。
- 容器：镜像的**运行实例**。装载实际运行程序的内容，实际镜像的步数。
- 仓库：存储镜像的位置。


## 1. Docker的安装

这里先从Linux开讲。


### 1.1 Linux
我使用的环境是Ubuntu，该发行版使用`apt`作为包体管理器。如果使用的是其他发行版，请按照自己的发行版进行安装。

需要安装以下必备物品：

```bash
sudo apt install apt-transport-https ca-certificates curl gnupg lsb-release
```

其中，这些工具的作用是：
- `apt-transport-https`：Docker的官方仓库（包括Docker本体）是通过`HTTPS`提供的，没有这个就无法安全访问。
- `curl`：获取Docker官方的密钥信息，必备。
- `ca-certification`：确保安装包体来自**可信任的**来源。
- `gnupg`：GNU Privacy Guard，管理密钥 - Docker的软件包使用GPG签名。
- `lsb-release`：获取当前OS发行版信息。Docker会为不同的linux发行版准备不同版本。

然后：
```bash
# 添加Docker官方GPG密钥
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg

# 添加Docker官方仓库
echo "deb [arch=amd64 signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

# 再次更新包索引
sudo apt update
```
该步骤用于添加Docker官方工具包仓库。


在配置完仓库后，安装：
```bash
# 安装Engine
sudo apt install docker-ce docker-ce-cli containerd.io

# 验证安装
sudo --version
```

### 1.2 Windows
需要确保自己的Windows版本支持虚拟化。*家庭普通版就不要想这个了，直接看Linux吧，搞个wsl还是有机会的。*

然后，直接登录Docker官网下载安装包，选择地址，一键安装并重启即可。

## 2. 基本使用教程：
以下镜像示例使用`postgres:latest`镜像作为演示，该镜像为Postgre SQL官方对外提供的镜像。

所有命令必须前缀：`docker`，在windows和linux上相同。

下表列举了使用镜像时的常用命令。

| 命令 | 速记 | 作用 | 常见用法 |
| ---- | ---- | ---- | ----- |
| `run` | Run  | 从镜像创建一个容器实例并启动 | 参数：`-d`后台运行，`-p`端口映射，`--name`命名容器 |
| `ps` | Process Status | 查看正在运行的容器 | 参数：`-a`查看所有容器（包括已停止的） |
| `images` | Images | 查看本地镜像列表  | - |
| `search` | Search | 搜索远程仓库镜像列表 | `search nginx`以输出带有关键词nginx的镜像 |
| `pull` | Pull | 从远程仓库拉取镜像 | 例：`pull nginx:latest` |
| `stop` | Stop | 停止运行中的容器 | `stop <容器ID/容器名>`  |
| `start` | Start | 启动一个已存在但停止的容器  | - |
| `volume` | Volume | 命名卷管理器（持久化需使用） | - |
| `rm` | Remove | 删除容器 | 参数：`-f`强制删除正在运行的容器 |
| `rmi` | Remove Image | 删除镜像  | - |
| `exec` | Exec | 在容器中执行命令 | 参数：`-it`交互式模式（常用于进入容器） |
| `logs` | Logs | 查看容器日志  | 参数：`-f`实时跟随日志输出 |
| `inspect` | Inspect | 查看容器基本信息 | 返回JSON信息，可用`--format`指定字段。 |

### 2.1 拉取镜像、启动容器的基本方法

#### 2.1.1 拉取镜像
在需要一个镜像之前，请搜索该镜像是否在远程仓库存在：
```bash
docker search postgres --limit 10
```
其中`search`的limit参数可限制最大输出镜像条目。

笔者在执行本指令时，终端输出：

```bash
NAME                   DESCRIPTION                                     STARS     OFFICIAL
postgres               The PostgreSQL object-relational database sy…   14550     [OK]
circleci/postgres      The PostgreSQL object-relational database sy…   34
cimg/postgres                                                          3
elestio/postgres       Postgres, verified and packaged by Elestio      1
kasmweb/postgres       Postgres image maintained by Kasm Technologi…   5
ubuntu/postgres        PostgreSQL is an open source object-relation…   41
mcp/postgres           Connect with read-only access to PostgreSQL …   27
chainguard/postgres    Build, ship and run secure software with Cha…   1
artifacthub/postgres                                                   0
corpusops/postgres     https://github.com/corpusops/docker-images/     0
```

拉取镜像时使用：
```bash
docker pull postgres:latest
```
其中，`postgres:latest`为：`镜像名:标签`，如果不写标签则默认为`latest`。
- 可以指定版本拉取镜像：
```bash
docker pull postgres:15
```
笔者使用标签为`latest`的内容进行拉取。


#### 2.1.2 基于拉取的镜像创建（并启动）容器
现在开始基于镜像启动容器。容器是基于镜像创建的实例，实例之间运行状态互相独立。

补充 - `run`指令的基本用法：
```bash
docker run [OPTIONS] IMAGE [COMMAND] [ARG...]
```
参数说明：
- `IMAGE`：要运行的镜像名称，例如`postgres:latest`。*如果镜像不存在，docker会自动下载之。*
- `[COMMAND]`：容器启动后执行的命令，例如`/bin/bash`或数据库启动命令。
- `[OPTIONS]` 常用选项：
  - `-d`：后台运行容器（detached mode）。
  - `--name <name>`：为容器指定名字，方便管理。
  - `-p <host_port>:<container_port>`：端口映射，把容器对外提供的端口映射到宿主机端口。
  - `-e KEY=VALUE`：设置环境变量（常用于数据库密码等配置）。
  - `-it`：交互式终端模式，常用于进入容器。
- `[ARG...]`：其他参数。

例：
```bash
docker run -d \
  --name=luna_pgsql \
  -e POSTGRES_PASSWORD=lunamoon \
  -p 1980:5432 \
  postgres:latest
```
其中，使用`-e`作为传入镜像`postgres:latest`的参数。*对于该镜像而言，该参数作用为设定登入数据库的默认用户`postgres`的密码为`lunamoon`。*

笔者将该指令写入了文件`run_docker.sh`，在启动后返回：
```bash
9ca5513b20b57543d67e4801c7806415b34448c1390a48a6ffbb1a8f4c2a9951
```
该串文本为运行时的容器编号全文。随后，使用`ps`指令确认当前正在运行中的容器。

```bash
$ docker ps
CONTAINER ID   IMAGE             COMMAND                  CREATED          STATUS          PORTS                                         NAMES
9ca5513b20b5   postgres:latest   "docker-entrypoint.s…"   27 seconds ago   Up 26 seconds   0.0.0.0:1980->5432/tcp, [::]:1980->5432/tcp   luna
```
容器创建成功。

现在，让容器内执行指令。
```bash
docker exec -it luna psql -U postgres
```
其中，`exec`指令的使用说明如下：
- `-U`：user，指定进入容器内的用户名，默认为`root`——但有些容器可能不认这个名字，所以请自行指定名字。
- `-i`：保持标准输入流（`stdin`）打开。速记：`interactive`。
- `-t`：为该指令分配一个伪终端。速记：`tty`。

```bash
docker exec -it luna psql -U postgres
psql (17.6 (Debian 17.6-1.pgdg13+1))
Type "help" for help.

postgres=# 
```
进入容器成功。

如果需要直接操作容器，可以使用：
```bash
docker exec -it luna sh
```
其中`sh`可以替换为`bash`。

#### 2.1.3 对已有镜像的启动和关闭
```bash
# 关闭容器
luna@[CENSORED]:~/docker$ docker stop luna
luna

# 查看容器状态
luna@[CENSORED]:~/docker$ docker ps -a
CONTAINER ID   IMAGE             COMMAND                  CREATED          STATUS                     PORTS     NAMES
438d52c69b84   postgres:latest   "docker-entrypoint.s…"   27 minutes ago   Exited (0) 4 seconds ago             luna

# 启动容器
luna@[CENSORED]:~/docker$ docker start luna
luna

# 再次查看
luna@[CENSORED]:~/docker$ docker ps
CONTAINER ID   IMAGE             COMMAND                  CREATED          STATUS          PORTS                                         NAMES
438d52c69b84   postgres:latest   "docker-entrypoint.s…"   27 minutes ago   Up 40 seconds   0.0.0.0:1980->5432/tcp, [::]:1980->5432/tcp   luna
```

### 2.2 容器数据的持久化
Docker容器本身是**临时的**。当容器停止或被删除后，容器内数据会**丢失**。

为实现数据持久化，Docker提供如下机制：
- 数据卷（`Volumes`）：储存在宿主机的特定目录里。容器停止或删除后数据仍保留。
- 绑定挂载（`Bind Mounts`）：允许将宿主机上的任意目录或文件挂载到容器内。
  - 如果容器和宿主机需要共享数据，推荐使用这一方法。该方法需要**手动**挂载路径。
- 临时文件系统（`tmpfs`）：将数据**存储在RAM**，需要用于高速IO但无需持久化的场景。
  - 适用于存储敏感数据或缓存。**容器停止或重启后，数据丢失**。

使用方法：（仅限在容器创建时使用）
```bash
docker run -v
```
笔者不推荐使用`-v`。该命令虽**适合快速挂载**，但后续语法不够明确，且配置选项有限。例：
```bash
docker run -v myvolume:/container/path ...
docker run -v /host/path:/container/path ...
```

推荐使用下述用法：
```bash
docker run --mount type=<type>,[source=<src>,]target=<target> ...
```
其中：
- `type`：可选值：`volume` `bind` `tmpfs`
- `source`：卷名（硬盘编号）或宿主机目录，当`type`字段为`tmpfs`时不需要`source`。
- `target`：在容器内的挂载点。

例：
```bash
docker run -d \
  --name=luna_pgsql \
  -e POSTGRES_PASSWORD=lunamoon \
  --mount type=bind,source="/home/luna/docker/docker",target="/mnt/attach_point"
  -p 1980:5432 \
  postgres:latest
```

Q：如果我已经创建容器，但**没在启动命令里设置持久化**，怎么办？
- A：从原本的镜像里重开一个容器吧。如果你舍不得旧数据，请使用：`volume`指令。（下表所有指令都需前缀`docker volume`）

| 指令 | 功能 | 表现 |
| --- | --- | --- |
| `create <volume_name>` | 创建一个卷，有名字的 | 容器可挂载该卷，实现数据持久化。 |
| `ls` | 列出当前已有的卷 | 包括命名卷和匿名卷。如果卷名是hash，则该卷为匿名卷。 |
| `inspect <volume_name>` | 查看卷信息 | - |
| `rm <volume_name>` | 删除卷 | - |
| `prune` | 删除未使用卷 | 一键清理未被任何容器使用的卷，匿名卷或命名卷都是。 |

注：在Linux系统里，默认存储位置为：`/var/lib/docker/volumes/<volume_name>/data`。
- 该内容笔者在WSL环境验证时发现：在Windows平台上，**Docker Desktop自带一个Linux VM运行Docker引擎**，导致宿主机的WSL2文件系统里并不存在该目录。
- 因此，该卷在Windows的存储位置实际在Docker Desktop的VM内，而不是WSL。

Q：如何查看当前容器正在使用的卷？
- A：请看VCR：
```bash
luna@[CENSORED]:~/docker$ docker inspect luna --format='{{json .Mounts}}'
[{"Type":"volume","Name":"54ce17165a71401e3e5420bebbe88f3b676cf0f2203eeb9322dbbf2a8e34c62e","Source":"/var/lib/docker/volumes/54ce17165a71401e3e5420bebbe88f3b676cf0f2203eeb9322dbbf2a8e34c62e/_data","Destination":"/var/lib/postgresql/data","Driver":"local","Mode":"","RW":true,"Propagation":""}]
```

Q：然后我怎么做？
- A：创建一个新容器，将旧容器使用的匿名卷挂到新容器上，然后再停止并删除旧容器即可。
  - 如果容器行为有创建匿名卷的话这样是可以的，但如果存储在容器内部的话……呃，请自行创建一个卷导出数据再导入吧。

## 3. 打包一个自己的镜像
镜像的本质是一个**只读模板**。

