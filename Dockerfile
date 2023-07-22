FROM gcc:11.2.0


RUN apt-get -qq update \
    && apt-get -qq install --no-install-recommends openssh-server \
    && apt-get -qq install --no-install-recommends sudo \
    && apt-get -qq install --no-install-recommends cmake \
    && apt-get -qq install --no-install-recommends rsync \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# setup ssh for use ubuntu, password 1234
RUN useradd -rm -d /home/ubuntu -s /bin/bash -g root -G sudo -u 1000 ubuntu 
RUN  echo 'ubuntu:1234' | chpasswd
RUN service ssh start
EXPOSE 22

# install ssh key
WORKDIR /root
RUN mkdir .ssh
WORKDIR /root/.ssh
RUN echo "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQDBnLge4YJ+iUyn29BHMvIm0G41q5j74b8QKwQpiMJubwXdEx/XjxA71WnqdKBz6QINawtzQHGjSkUS/ReIeL+eT7NznHWN+tt7mQtadTzcoKUrPNIrG5+SxxrWHtafGJ6ky5KACCFPmzjTXlGHuvxHl3Njy2aF63MgiZBtQITai7rXbH/oDdLZGWdPvh3jJLtmx0SSrwYBelzdPv7kcPQHgVRdTTOKrLT1XUhzs89y4D9NHMW3qoRqmybgI7Zsv10St8YBP2sK7v2ONiNlwtrNZhTCGWmiHq/qG52Ih/0kr/jskT1n1DDF/NZ89LIp2txmml7bXbXaItN5CLLiW9EA9wZ7J5saZj3JCbREa5PERcIo/FJfs4awRbi4bFgU4B8d41WHB5NLoaiHZhaZvGEYdkYZ1zxQLqg+1NNjsR2HdXnG586P23x7blFjKlyNAfHDZyAyYJOHoI0wpuzDH9EAEJDEfSyF6oZbb7Uw2+FoUEYDYOY2KV5AXs8yJt6eahM= root@d27772bb6a43" > id_rsa.pub
RUN echo "-----BEGIN OPENSSH PRIVATE KEY-----\n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\n-----END OPENSSH PRIVATE KEY-----" > id_rsa
RUN echo "|1|TDTJWiBvb/qiHTkqsL0HcrX+8uQ=|l/7WBBh26xOWtIIMILcJs8o2x5E= ecdsa-sha2-nistp256 AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBEmKSENjQEezOmxkZMy7opKgwFB9nkt5YRrYMjNuG5N87uRgg6CLrbo5wAdT/y6v0mKV0U2w0WZ2YB/++Tpockg=\n|1|BUn2uhqf2tUStjuLE4Sv+JIQq6o=|IlxlFdB1T4KzUI/TE1c5q7E/Deg= ecdsa-sha2-nistp256 AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBEmKSENjQEezOmxkZMy7opKgwFB9nkt5YRrYMjNuG5N87uRgg6CLrbo5wAdT/y6v0mKV0U2w0WZ2YB/++Tpockg=" > known_hosts
RUN chmod 400 ~/.ssh/id_rsa

# # install google test
WORKDIR /usr/src/libraries
RUN git clone --depth=1 -b main git@github.com:google/googletest.git
WORKDIR /usr/src/libraries/googletest/build
RUN cmake .. \
    && make \
    && make install

# build the project
WORKDIR /usr/src/app
COPY . .
RUN rm -rf build
RUN mkdir build
WORKDIR /usr/src/app/build
RUN cmake ..
RUN make

# CMD ["/usr/sbin/sshd","-D"]
# CMD ["./main"]
