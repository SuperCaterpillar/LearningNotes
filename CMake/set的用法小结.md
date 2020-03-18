## set用法小结

```cmake
set(test FALSE CACHE BOOL "ssdkfjdg")
if(test)
    add_executable(demo1 src/demo.cpp) # 生成可执行文件
else()
    add_executable(demo2 src/demo2.cpp) # 生成可执行文件
endif()
```

 cmake ../ -Dtest=0 