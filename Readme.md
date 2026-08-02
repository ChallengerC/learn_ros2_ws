### clangd文件生成
```
colcon build --cmake-args -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```
```
colcon build --cmake-args -DCMAKE_BUILD_TYPE=Debug
```

### 编译制定包
```
colcon build --packages-select 包名 --symlink-install
```

### 安装编译依赖
```
rosdep install --from-paths src --ignore-src -r -y
```