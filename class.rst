Class
#####

Details
********

APIs
*****

void mrb_define_method_raw(mrb_state\*, struct RClass\*, mrb_sym, struct RProc \*)
===================================================================================

* RProc 構造体を使ってクラスメソッドを定義する

  * mrb_define_method() では関数ポインタをメソッドの定義として渡していたが、こちらは RProc
  * クラスの kh（内部的なハッシュ、で合ってる？）に第三引数のシンボルのキーに対し RProc ポインタをセット

struct RProc \*mrb_method_search(mrb_state\*, struct RClass\*, mrb_sym)
===========================================================================

* RiteVM 中で定義済みのメソッドを探索する
* 中で mrb_method_search_vm() を呼んでる

  - メソッドが見つからなかった場合の追加処理がある

mrb_method_search_vm()
======================================

* クラス内に定義されるメソッドを、 khash をなめることで探索する
* 見つからなかったら親クラスをたどる
