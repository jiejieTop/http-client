# 如何提交PR

本文尽量使用命令行工具介绍如何向开源项目提交 **PR (Pull Request)**，一次亲身经历提交 **PR** ！

## fork项目

在浏览器打开 [https://github.com/jiejieTop/httpclient](https://github.com/jiejieTop/httpclient) ，点击右上角的`fork`按钮，`fork`开源项目到你自己的`github`账户。

![how_to_pr001](http://qiniu.jiejie01.top/how_to_pr001.png)

然后会自动进入你自己的仓库，开源看到相关的信息：

![how_to_pr002](http://qiniu.jiejie01.top/how_to_pr002.png)

接着你点击【Clone or download】 按钮，会出来一个链接，点击②复制该链接（也可手动复制）。

## clone项目

然后进入linux平台，打开终端，运行以下命令：

```bash
git clone [你复制的链接]
```

## 添加上游源仓库

添加上游源仓库是为了保证与原始仓库保持一致，因为原始仓库一直保持更新，使用以下命令添加上游源仓库:

```bash
git remote add upstream https://github.com/jiejieTop/httpclient.git
```

添加完成后可以使用`git remote -v` 查看当前的远程仓库地址信息:

```bash
git remote -v

origin  https://github.com/iot-strategy/httpclient.git (fetch)
origin  https://github.com/iot-strategy/httpclient.git (push)
upstream        https://github.com/jiejieTop/httpclient.git (fetch)
upstream        https://github.com/jiejieTop/httpclient.git (push)
```

## 更新源仓库

之后运行下面几条命令，就可以保持本地仓库和上游源仓库同步了

```
git fetch upstream
git checkout master
git merge upstream/master
```

## 修改代码，添加新功能

你可以选择切换分支开发也可以选择在master分支上进行开发，在开发完成后，可以提交到你自己的远端仓库。

接下来就是正常的 `git add ` 、 `git commit` 、 `git push `操作了，此处不做过多叙述。






