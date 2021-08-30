# http
## keep-Alive 和 非keep-Alive的区别
- http 1.0 每次发起请求 都要建立新连接， 完成之后断开。
- http 1.1 中 默认使用持久连接， 而老版本都是默认非持久连接
- 为了在老版本里弄持久连接，就要在connection里制定 keep-alive 让他用完不要关
- 设置keepalive - timeout, 响应完最后一个请求后 倒计时关闭
- 应用场景
  - 长连接
    - 操作频繁，点对点通讯，客户端数目较少
    - 即时通讯，游戏
  - 短连接
    - 用户数目较多，要控制连接总数的
    - 京东 淘宝等 大型网站

## http 方法
- http/1.0: get, post, head
- http/1.1: get, post, head, options, put, patch, delete, trace, connect
- get
  - 请求指定的页面信息，返回具体内容
- post
  - 提交数据，请求处理。
  - 数据被包含在request body里
  - 会导致 新的资源的建立 或 已有资源的更改
- put
  - 替换制定资源 没有的话就新增
- delete
  - 请求服务器删除url标识的资源
- head
  - 和 get 类似， 返回的响应中没有具体内容，用于获取报头
- connect
  - 将服务器作为代理，让服务器代替用户进行访问
- options
  - 问问服务器，可以对这个资源做什么 (返回get, put这种)
- trace
  - 让服务器返回 他刚刚收到了个什么， 用来测试诊断的
- patch
  - 和put差不多，对已知资源 局部更新

- *get 和 post的区别*
  - get提交的数据 会放在url 后面 以query的形式在整个链接里。query参数会被浏览器保留。直接暴露，可能会有安全问题
    - query形式：xxx?key=value&key=value

  - post参数放在request body里。参数不会被保留。没有直接暴露，会好一点
    - post body可以加密

  - get 只支持url编码，post 请求支持多种编码格式
  - get 只支持ascii码，所有字符都会被转成ascii。 post 无所谓
  - get 的数据大小有限制，post 没限制
  - 对于get
    - 浏览器会把http header 和data 一起发出去，响应200 说明成功
  - 对于post 
    - 浏览器会先发送http header 给服务端，告诉服务端等一下会有数据过来， 服务端响应100 continue, 告诉他ok准备好了
    - 浏览器再post 一个data 给服务端， 服务端响应200,搞定
  
  - get 长度被浏览器限制，看浏览器， ie 2000个字符， chrome 8182个字符，
  - post body大小没有限制，只要你搞得下。不过其实也没人会发一个巨大的包

## 状态码
- 1XX       指示信息， 表示请求正在被处理
- 2XX       成功
- 3XX       重定向， 要完成的请求需要附加操作
- 4XX       客户端错误
- 5XX       服务端错误

- 1XX
  - *100     continue            继续*
  - 101     switching protocol  服务器根据客户端的请求切换到更高级的协议
- 2XX
  - 200     OK                  返回成功
  - *201     Created             创建成功*
  - 202     Accepted            接受请求，但我这里还没搞完
  - 203     未授权               请求成功，返回的东西不在服务器里，而是一个副本
  - 204     No Content          无内容，处理成功了。但服务器不用返回你东西
  - 205     Reset Content       重置内容。也没返回你东西。但是要求你这里页面更新一下
  - 206     Partial Content     成功处理了部分get
- 3XX
  - *300     Multiple choices    多种选择。get的东西有好几种选择，用户你自己挑一个来重定向*
  - *301     Moved Permanently   永久移动。这个东西地址换了，我会返回你现在的地址。浏览器会自己转到新地址*
  - 302     Found               临时移动。还是会给你转到临时地址。但你以后要查还是先查老地址
  - 303     See Other           查看其他地址。也是换新地址了。使用Get post请求查看
  - 304     Not Modified        未修改。如果客户端发送了一个带条件的get 且被接受 而文档的内容还没修改 返回这个
  - 305     Use Proxy           使用代理。必须使用指定的代理才能访问这个
  - 307     Temporary Redirect  和302一样。
- 4XX
  - *400     Bad request         请求语法有问题*
  - *401     Unauthorized        没有权限*
  - *403     Forbidden           不让访问这个*
  - *404     Not Found           没有这个资源*
  - 405     Method not Allowed  这个方法被禁止
  - 406     Not acceptable      资源和请求头的内容特性不匹配。不能形成相应实体
  - 407     Proxy Auth Required 需要在代理服务器上验证身份
  - ...
- 5XX
  - *500     internal server     不知道哪里出错了 无法完成处理*
  - 501     Not implemented     这个功能服务器没有
  - 502     Bad Gateway         充当网关或代理的服务器，从远端服务器收到了一个无效请求
  - 503     Service Unavail     服务器在维护中 或者挂了
  - 504     Gateway Timeout     充当或代理的服务器，未及时从远端服务器获取请求
  - 505     http ver not supp   服务器不支持这个http版本


## https工作流程
1. 客户端发送 https 请求， 连接到443 端口  *给对称加密列表*
  - 内容： 自身支持的算法列表，密钥长度
2. 服务端 挑一种 双方都支持的加密算法， 连同密钥组件，一起发给客户端    *挑一种*
3. 服务端 发送 CA 证书， 包含 颁证机构，过期时间，服务端公钥    *给你看下我身份证*
4. 服务端 发送 完成报文 通知客户端SSL的第一阶段 已经协商完成 用哪个加密算法   *协商完成用哪个加密算法*
5. 客户端 发送一个回应报文， 包含一个 随机密码串， pre_master_secre， 这个是被公钥加密过的  *公钥加密对称密钥*
6. 客户端 发送一个报文 提示服务端 此后的报文 都是采用pre_master_secre 加密的    *开始当谜语人了！*
7. 客户端 发送一个finish 报文， 包含第一次握手至今所有报文的 整体checkSum。 最终协商是否完成，取决于服务端能否成功解密  *说一串谜语，看看你能懂吗*
8. 服务端 发送一个报文 提示客户端 此后都是 采用 pre_master_secre 加密的， 最后发送finish 报文，告诉自己可以正常解密  *能懂，现在开始全体谜语人*

- 为什么相信CA证书 *还没弄懂*
  - 如果CA证书被篡改了， 篡改方没有私钥，所以他没办法得到加密后的签名，因此无法做对称加密
  - 客户端 收到被篡改的证书后，发现原文和签名解密后的值不一致，说明证书被中间人篡改，证书不可信，有内鬼停止交易。

- http 与 https 区别
  - http 全体明文， https 全体谜语
  - 80端口， 443端口
  - https 需要经过TLS协商，建立连接会慢一点


## 各http版本
### http/1.0 与 http/1.1区别
- 缓存处理
  - http/1.0 中 使用header里的 if modified-since expires 来判断缓存情况
  - http/1.1 添加很多新的缓存策略, entity-tag, if-unmodified-since, if-match, if-none-match
- 节约带宽
  - http/1.0 默认将资源相关的整个对象传送给请求方。 不管是不是全要。
  - http/1.1 请求头里 加入 range 域，可以部分请求资源。 方便多线程
- 增加错误通知
  - http/1.1 新增24个错误状态。
- host 请求头
- 长连接
  - http/1.0默认短连接，每个请求专门弄个tcp, 用完关掉
  - http/1.1 默认长链接。

### http/1.x 和 http/2.0区别
- http/1.x 用字符串传送
- http/2.0 传送binary。数据分成帧 frame, 帧组成了数据流， 数据流具有流ID标识和优先级。通过优先级和流依赖，解决关键请求被阻塞的问题
- http/2.0 支持多路复用。
  - 因为流ID的存在，通过同一个http请求，实现多个http请求传输。客户端和服务器通过流ID来标识 谁是谁，从而定位来自哪个http请求
- 头部压缩
  - http/2.0 通过gzip 和 compress 压缩头部然后再发送。
  - 通信双方维护一张头信息表，所有字段都记录在这张表中
  - 每次httpc传输时只需要传 头字段 在表中的索引 就可以了。 减少每次传输的数据量
- 服务器推送
  - 服务器可以在未经请求许可的情况下，向客户端推送内容

### http3
- http2的问题
  - 需要三次握手
  - 加密需要 TLS
  - tcp 慢启动， 拥塞窗口

- Quic 协议 == quick udp internet connections
  - 快速udp
  - 满足传输层和应用层 对多连接， 低延迟 的需求
  - 融合了 tcp, TLS, http/2的特性，基于 UDP 传输
  - 好处
    - 低延迟连接
      - QUIC 只需要 一个rtt 就可以建立安全可靠的连接， 比TCP + TLS 的3次rtt要快
      - 客户端缓存加密认证的信息。 再次连接可以直接连接， 0 rtt
    - 多路复用
      - 采用http/2的多路复用的功能。 由于quic 基于udp, 也避免了http/2存在的队头阻塞问题
    - 加密认证
      - quic报文头部经过认证，报文主体经过加密
      - 接受端能发现quic报文是否被篡改
    - 向前纠错机制
      - 每个数据包 携带部分其他数据包的内容。
      - 使出现少量丢包的情况下，尽量减少其他包的重传次数
      - 牺牲单个包所携带的有效数据大小，换来更少的重传次数

- http/3
  - 基于quic 发展而来
  - 底层使用udp, 上层使用http/2
  - udp 和 http/2之间 存在quic层， TLS在quic层处理
  - 特点
    - 使用UDP作为传输层
    - quic层完成TLS加密握手，保证http/3 的安全性
    - 建立连接快， 1个rtt即可建立连接
    - 不和具体底层连接绑定
      - quic 为每个连接的两端 分配唯一ID
      - 上层连接只认准 这两个逻辑ID
      - 网络切换或断连时，只需要继续发送数据包，即可完成连接的建立
      - 使用Qpack 进行头部压缩，避免队头阻塞 
