#!/bin/bash
# 安装 Docker
sudo apt-get update
sudo apt-get install -y ca-certificates curl gnupg lsb-release alsa-utils squeezelite
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu \
  $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update
sudo apt-get install -y docker-ce docker-ce-cli containerd.io docker-compose
sudo systemctl start docker

# 清理缓存
apt autoremove -y && apt autoclean && apt remove -y && apt clean
