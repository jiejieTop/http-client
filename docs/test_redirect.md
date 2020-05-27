# 重定向测试

当 **http** 服务器的资源被 **挪移** 到其他地方的时候，可能会返回一个 **重定向的 url**，状态码可能是 **30X** ，此时我们的拦截器需要正确处理这个 url，并重新取得相应的资源，这就是重定向，这个功能是在拦截器中处理的，对用户来说相当于透明。

## 动态图演示

![redirect](http://qiniu.jiejie01.top/redirect.gif)

## 代码

```c
#include <stdio.h>
#include <unistd.h>
#include <httpclient.h>

extern const char *ca_get();

#define URL1 "http://httpbin.org/redirect-to?url=http://www.jiedev.com&status_code=301"         // tcp
#define URL2 "https://httpbin.org/redirect-to?url=http://www.jiedev.com&status_code=301"         // tls

static int _http_cb(void *e)
{
    http_event_t *event = e;

    printf("%s", (char*)event->data);

    return 0;
}

void http_redirect_test(void)
{
    printf("\n---------------------- http_redirect_test start ----------------------\n");

    http_client_init(ca_get());
    
    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
    http_client_get(URL1, _http_cb);
    printf("\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");

    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
    http_client_get(URL2, _http_cb);
    printf("\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");

    printf("\n---------------------- http_redirect_test end ----------------------\n");

}
```

## 现象

从代码我们很明显可以看出，我们访问的是 **httpbin.org** 这个服务器，但是此时资源已经被挪移到 `www.jiedev.com` 服务器了（纯粹是为了测试，并非真正挪移），那么拦截器就要从 `www.jiedev.com` 服务器获取新的资源。

```bash
---------------------- http_redirect_test start ----------------------


>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

<!doctype html>
<html>
<head>
    <meta charset="utf-8">
    <title>富强、民主、和谐</title>
    <style>
        .container {
            width: 60%;
            margin: 10% auto 0;
            background-color: #f0f0f0;
            padding: 2% 5%;
            border-radius: 10px
        }

        ul {
            padding-left: 20px;
        }

            ul li {
                line-height: 2.3
            }

        a {
            color: #20a53a
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>祝祖国70周年生日快乐</h1>
        <h3>爱祖国，为祖国的前途而奋斗，是时代赋予我们的神圣职责</h3>
        <ul>
          <li>我是中国人民的儿子。我深情地爱着我的祖国和人民。</li>
          <li>谁不爱自己的母亲，用那滚烫的赤子心灵。谁不爱自己的母亲，用那滚烫的赤子心灵。
              亲爱的祖国，慈祥的母亲，长江黄河欢腾着，欢腾着深情，我们对您的深情。</li>
          <li>我游览了白山黑水，看到了祖国的繁荣和富足。登上南沙群岛的珊瑚礁啊！我感受到了祖国幅员的广袤。
              在八百里秦川，我听到了祖国儿女气势奔雷的心声。那一阵阵响入云天的威风锣鼓，那一声声惊天动地的浑厚呐喊。</li>
          <li>“苟利国家生死以，岂因祸福避趋之”，我们现在就应该脚踏实地的从身边平凡的小事做起，不断陶冶爱国之心，增强爱国之智，将来为我们的祖国贡献力量！<br>
              爱国，是一种坚定的民族精神，是一种振兴中华的责任感。我们青少年要用这种强大的精神力量去描绘祖国未来的宏图，将一片丹心献给祖过。让我们一起向着“心系祖国，健康成长”的目标前进吧！<br>
              爱国，是至高无上的品德、是民族的灵魂。亲爱的同学们，为了学校的荣誉、为了我们肩上的重任，为了中华民族的腾飞，让我们努力学习，做一个无愧于人民的具有爱国主义情怀的万里学子吧！<br>
              爱国是一个人对自己祖国的一种诚挚的热爱和深厚的情感，是一个人最原始的感情之一。但作为一个涉世不深的我，一直认为爱国是高不可攀也是比较渺茫的事。最近，我读了《民族英雄郑成功》一书，对爱国之情有了深刻的感悟。</li>
        </ul>
    </div>
</body>
  
        <br><br><br><br><br><br><br>
        <p></p>
        <p1 style="font-size: 11px; color: darkgray; margin:0px 0px -15px 0px; "><center>Copyright © 2019 杰杰 <a href="http://beian.miit.gov.cn">粤ICP备19019080号</a></center></p1>
        </div>
        <span id="momk"></span><br><br><br><br><br>
</html>

<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

<!doctype html>
<html>
<head>
    <meta charset="utf-8">
    <title>富强、民主、和谐</title>
    <style>
        .container {
            width: 60%;
            margin: 10% auto 0;
            background-color: #f0f0f0;
            padding: 2% 5%;
            border-radius: 10px
        }

        ul {
            padding-left: 20px;
        }

            ul li {
                line-height: 2.3
            }

        a {
            color: #20a53a
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>祝祖国70周年生日快乐</h1>
        <h3>爱祖国，为祖国的前途而奋斗，是时代赋予我们的神圣职责</h3>
        <ul>
          <li>我是中国人民的儿子。我深情地爱着我的祖国和人民。</li>
          <li>谁不爱自己的母亲，用那滚烫的赤子心灵。谁不爱自己的母亲，用那滚烫的赤子心灵。
              亲爱的祖国，慈祥的母亲，长江黄河欢腾着，欢腾着深情，我们对您的深情。</li>
          <li>我游览了白山黑水，看到了祖国的繁荣和富足。登上南沙群岛的珊瑚礁啊！我感受到了祖国幅员的广袤。
              在八百里秦川，我听到了祖国儿女气势奔雷的心声。那一阵阵响入云天的威风锣鼓，那一声声惊天动地的浑厚呐喊。</li>
          <li>“苟利国家生死以，岂因祸福避趋之”，我们现在就应该脚踏实地的从身边平凡的小事做起，不断陶冶爱国之心，增强爱国之智，将来为我们的祖国贡献力量！<br>
              爱国，是一种坚定的民族精神，是一种振兴中华的责任感。我们青少年要用这种强大的精神力量去描绘祖国未来的宏图，将一片丹心献给祖过。让我们一起向着“心系祖国，健康成长”的目标前进吧！<br>
              爱国，是至高无上的品德、是民族的灵魂。亲爱的同学们，为了学校的荣誉、为了我们肩上的重任，为了中华民族的腾飞，让我们努力学习，做一个无愧于人民的具有爱国主义情怀的万里学子吧！<br>
              爱国是一个人对自己祖国的一种诚挚的热爱和深厚的情感，是一个人最原始的感情之一。但作为一个涉世不深的我，一直认为爱国是高不可攀也是比较渺茫的事。最近，我读了《民族英雄郑成功》一书，对爱国之情有了深刻的感悟。</li>
        </ul>
    </div>
</body>
  
        <br><br><br><br><br><br><br>
        <p></p>
        <p1 style="font-size: 11px; color: darkgray; margin:0px 0px -15px 0px; "><center>Copyright © 2019 杰杰 <a href="http://beian.miit.gov.cn">粤ICP备19019080号</a></center></p1>
        </div>
        <span id="momk"></span><br><br><br><br><br>
</html>

<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


---------------------- http_redirect_test end ----------------------
```

**上一篇**：[post 方法测试](./test_post.md)

**下一篇**：[获取文件测试](./test_get_file.md)
