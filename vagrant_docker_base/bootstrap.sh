#!/bin/bash
echo "替换国内镜像..."
mv /etc/apt/sources.list /etc/apt/sources.list.bak
cp /vagrant/sources.list /etc/apt/sources.list
mkdir -p /etc/docker
cp /vagrant/daemon.json /etc/docker/daemon.json
apt-get update

echo "卸载Docker老版本..."
apt-get remove docker docker.io

echo "安装Extra Packages..."
apt-get -f install -y linux-image-extra-$(uname -r) linux-image-extra-virtual

echo "安装HTTPS需要的包..."
# 参考 https://yq.aliyun.com/articles/110806
apt-get -f install -y apt-transport-https ca-certificates curl software-properties-common
curl -fsSL http://mirrors.aliyun.com/docker-ce/linux/ubuntu/gpg | apt-key add -

echo "安装Docker CE..."
sudo add-apt-repository "deb [arch=amd64] http://mirrors.aliyun.com/docker-ce/linux/ubuntu $(lsb_release -cs) stable"
apt-get update
apt-get -f install -y docker-ce

echo "添加vagrant用户到docker用户组"
usermod -a -G docker vagrant

echo "安装libcap-dev，用于验证docker用户隔离示例代码"
apt-get -f install -y libcap-dev