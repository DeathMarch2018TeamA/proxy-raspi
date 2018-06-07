# proxy-raspi

---

## 概要

本リポジトリは[PEAR Lab](http://www.is.utsunomiya-u.ac.jp/pearlab/ja/)の導入教育プログラム"DeathMarch2018"で開発したものです．

### proxy-raspiとは

サーバとクライアント間のデータを中継します．サーバから送られるブロックと2次ハッシュに要求するゼロの個数をクライアントへ送信し，クライアントから送られるナンス値をサーバへ送信します．

##ビルド方法

本リポジトリをクローンし，srcディレクトリ内でmakeコマンドを実行するとビルドできます．

```
git clone https://github.com/DeathMarch2018TeamA/proxy-raspi.git
cd proxy-raspi/src
make
```

ビルドに成功したら，binディレクトリ内に実行ファイルが生成されます．
