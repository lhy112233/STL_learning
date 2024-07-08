```C++
struct obj{
    ~obj(){
        try{
        if(fd) fd.close();  //可能抛异常
        }catch(...){...吞掉异常};
    }

    void close(){fd.close();}   //可能抛异常，但避免了在析构中抛异常的可能.

    file fd;
};

```

