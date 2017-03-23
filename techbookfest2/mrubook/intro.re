= はじめに

本書は軽量 Ruby 実装である mruby の言語処理系の実装の解説本です。
具体的には mruby のコンパイラや VM 、コア機能（本書ではハッシュテーブルと GC にフォーカス）について触れていきます。

想定読者は Ruby でコードを書いたことがある人、 Ruby の言語処理系に興味がある人としています。
そのため本書では Ruby の文法についてゼロから説明するなどといった記述はしません。
あらかじめご了承ください。

本書が、あなたにとって mruby に関する理解を深めたり mruby コミュニティに参加していくきっかけになれたら嬉しいです。

== mruby とは

さっそく mruby の実装の話に突入したいところですが、流石に急すぎると思われるので簡単に mruby 自体について解説いたします。
あなたが「mruby は既にある程度知ってるよ！」という方でしたら、次の章までスキップしていただければ幸いです。

mruby は先述の通り軽量、省メモリ Ruby の実装です。
軽量な処理系の実現のため、標準の機能がある程度 Ruby から削減されています。
具体的には Ruby の JIS/ISO 規格に記載がある機能は標準で搭載されているのですが、それ以外はオプションな立ち位置で組み込むが ON/OFF が可能だったり、そもそも実装されていなかったりします。

主な用途としては組み込み機器での利用や他のアプリケーションに組み込んでの利用が想定されます。
他のアプリケーションに組み込んで使う例としては、 matsumoto-r さんが開発した mod_mruby や ngx_mruby などが挙げられます。
これはそれぞれ Apache HTTP Server と Nginx で設定ファイルに書くような内容や簡単な制御ロジックを mruby のスクリプトで記述することを可能にするモジュールです。
その他にも、 mruby-cli を使ってコマンドラインツールを実装するための言語として mruby を使用するパターンがあったりします。

mruby のサードパーティライブラリは mrbgem という単位で管理・配布可能です。ちょうど Ruby の RubyGems に相当します。
mrbgem は mruby のビルド設定ファイルに記述を追加することで、ビルドされたバイナリにリンクされる形で取り込まれます。
mrbgem は mruby 界隈に関わる企業、有志によって既に多くの数が提供されています。
探してみると、 Ruby では標準ライブラリとして存在したが mruby では削られてしまったものが mrbgem として提供されていたり、あるいは mruby 特有の mrbgem が提供されていたりします。
具体的には @<table>{mrbgems} のような mrbgem が提供されています。
これらはあくまでも一部抜粋で、実際には GitHub などを探索するともっと膨大な数の mrbgem が見つかるはずです。

//tsize[30,80]
//table[mrbgems][mrbgems の一部紹介]{
名前	内容 
-------------------------------------------------------------
mruby-http	HTTP パーサ
mruby-io	IO や File クラスを提供
mruby-json	JSON パーサ
mruby-msgpack	MessagePack ライブラリ
mruby-redis	Redis クライアント
mruby-uv	libuv インタフェース
//}

== おことわり

本書は mruby-1.2.0 をベースに執筆しています。
ソースコードと照らし合わせないと分かりにくい箇所が多々あるため、ぜひお手元のマシンにこれを用意し、ソースコードと合わせて本書を読み進めてみていただけると幸いです。
残念ながら mruby-1.2.0 は本書執筆時の一年以上前にリリースされたですので、本書で解説する内容は最新のソースコードの内容と齟齬が発生している可能性があります。

また、本書は網羅的であることを目指してはいません。
本書ではいくつか他の（素晴らしい出来の！）書籍を紹介していきます。
もしそういった箇所で興味をお持ちいただけたら、ぜひそれら他の文書も合わせて閲覧いただければと思います。

