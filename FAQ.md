答：Libfetion-gui-wince是基于Libfetion的Windows Mobile版，包括SP和PPC系统的支持。目前也有M8版本。Libfetion是由邓东东主导的第三方飞信软件，目前有PC版和Linux版。

> 问：libfetion好使吗?
答：只有您体验过才知道是否好用！因为好用的标准是由您评价的。

> 问：使用libfetion WM版登陆为什么显示PC在线？
答：libfetion使用的PC端飞信协议，所以将看到PC方式登录。当然飞信积分也按照PC客户端计算。

> 问：将来是否会支持CMWAP方式？
答：libfetion-gui-wince这个项目不会支持cmwap方式，因为libfetion使用PC端协议，登陆必须使用TCP协议，而移动cmwap接入点做了限制，只能发起HTTP请求，所以libfetion无法使用！
如果您需要使用cmwap接入点，建议使用官方原版飞信。

> 问：如何使用自定义提示音？
答：将如下指定的提示音文件放到LibFetion程序同目录下就可以实现自定义提示音。
> > 新消息提醒                      message.wav
> > 好友上线提醒                   online.wav
> > 群消息提醒                      qun.wav