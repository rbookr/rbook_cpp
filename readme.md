# rbook implement by c++

- [rainboyOJ/markdown-it-cpp](https://github.com/rainboyOJ/markdown-it-cpp)
- b树


## API设计

```
得到所有的书数据 
GET /get/booklists
返回值:
{
    status: 0,
    msg:'ok',
    books:[
        {
            icon:
        }
    ]
}
```


```
一本书
/get/book/:name/
有可能返回的子列表[可能有多个目录,也可以只有一个]
```

```
一本书的内容目录
/get/book/:name/catlog?
```

```
一篇文件的内容

/get/book/:name/id?
{
    status:0,
    msg:'ok',
    content: 'html_raw',
    others:?? yaml_info ??
}
```

## 设计

四个模块

```
后端: Net,渲染,数据存储
前端: UI(vue3 tailwind)
```
