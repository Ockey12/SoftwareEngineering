#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int enterSize(float*);
int enterWeight(float*);


int main(){

  // サイズの合計
  float size = 0;
  // 重さ
  float weight = 0;
  // 大きさによる配送料金
  int sizeCharge = 0;
  // 重さによる配送料金
  int weightCharge = 0;
  // 関数の返り値を格納
  int flag = 0;

  printf("サイズを入力してください。\n");

  // 縦を入力
  // 正しく入力されるまで繰り返す
  while(1) {
    printf("縦(cm)  :");
    flag = enterSize(&size);
    if(flag == 1) {
      // 1は無効な入力なので、このループをスキップして次のループで再入力
      continue;
    } else if(flag == 2) {
      // 2は取り扱えないサイズなので0を返して終了
      return 0;
    } else {
      // 正しく入力されているのでループを抜ける
      break;
    }
  }

  // 横を入力
  // 正しく入力されるまで繰り返す
  while(1) {
    printf("横(cm)  :");
    flag = enterSize(&size);
    if(flag == 1) {
      continue;
    } else if(flag == 2) {
      return 0;
    } else {
      break;
    }
  }

  // 高さを入力
  // 正しく入力されるまで繰り返す
  while(1) {
    printf("高さ(cm):");
    flag = enterSize(&size);
    if(flag == 1) {
      continue;
    } else if(flag == 2) {
      return 0;
    } else {
      break;
    }
  }

  // 重さを入力
  // 正しく入力されるまで繰り返す
  while(1) {
    printf("重さ(kg):");
    flag = enterWeight(&weight);
    if(flag == 1) {
      continue;
    } else if(flag == 2) {
      return 0;
    } else {
      break;
    }
  }

  // 大きさによる料金を決定
  if(size <= 60) {
    sizeCharge = 1040;
  } else if(size <= 80) {
    sizeCharge = 1260;
  } else if(size <= 100) {
    sizeCharge = 1500;
  } else if(size <= 120) {
    sizeCharge = 1720;
  } else if(size <= 140) {
    sizeCharge = 1960;
  } else {
    sizeCharge = 2180;
  }

  // 重さによる料金を決定
  if(weight <= 2) {
    weightCharge = 1040;
  } else if(weight <= 5) {
    weightCharge = 1260;
  } else if(weight <= 10) {
    weightCharge = 1500;
  } else if(weight <= 15) {
    weightCharge = 1720;
  } else if(weight <= 20) {
    weightCharge = 1960;
  } else {
    weightCharge = 2180;
  }

  puts("配送料金は以下のとおりです。");

  // 高い方の料金を表示
  if(sizeCharge > weightCharge) {
    printf("%d円\n", sizeCharge);
  } else {
    printf("%d円\n", weightCharge);
  }

  return 0;
}


// 縦・横・高さのいずれかの入力とエラー処理、合計の加算を行う
// return 0 : 正常に終了
// return 1 : 無効な入力があった
// return 2 : sizeの合計が160を超えた
int enterSize(float *size){
  
  // 入力された文字列を格納
  char buffer[7];
  // float型に変換できなかった文字へのポインタを格納
  char *endptr;
  // 入力をfloat型に変換した数値を格納
  float num = 0;
  
  // 入力の先頭から、改行を除いた6文字のみを受け入れる
  // 長さをmmまで測定するとすると、「160.0」の5文字で十分
  // 「0.001」と入力できるが、float型なので160cmを超えていないか十分判定できる
  // 改行を除かないと、改行のみの入力も受け入れてしまう
  // アルファベットなどの無効な入力を検知できるよう、受け入れられる文字数に余裕を持たせている
  // 1文字以上読み取れたときは1を返すので、1以外が返されたら無効な入力として1を返して異常終了
  if(scanf("%6[^\n]%*[^\n]", buffer) != 1) {
    // 次のscanf()のために、残った改行を読み取って捨てる
    // 改行を削除しておかないと無限ループになる
    scanf("%*c");
    puts("何も入力されていません。測定した長さを入力してください。");
    return 1;
  }
  // 次のscanf()のために、残った改行を読み取って捨てる
  scanf("%*c");

  // 6文字以上入力された場合
  // 入力し直すよう促すメッセージを表示
  // 1を返して異常終了
  if(strlen(buffer) == 6) {
    puts("入力が長すぎます。入力し直してください。");
    return 1;
  }

  // 入力をfloat型に変換
  // &endptrには、変換できなかった文字へのポインタを格納
  num = strtof(buffer, &endptr);

  // 変換できない文字があった場合
  // 正しく入力するよう促すメッセージを表示
  // 1を返して異常終了
  if(*endptr != '\0') {
    puts("無効な文字が含まれています。測定した長さを数字のみで入力してください。");
    return 1;
  }

  // 0以下の数値が入力された場合
  // 0より大きい数値を入力するよう促すメッセージを表示
  // 1を返して異常終了
  if(num <= 0) {
    puts("無効な入力です。0より大きい長さを入力してください。");
    return 1;
  }

  *size += num;
  
  // 縦・横・高さの合計が160を超えていた場合
  // 取り扱えないことを伝えるメッセージを表示
  // 2を返して異常終了
  if(*size > 160) {
    puts("箱の3辺の長さの合計が160cmを超えているので、取り扱うことができません。");
    return 2;
  }

  return 0;
}


// 重さの入力とエラー処理を行う
// return 0 : 正常に終了
// return 1 : 無効な入力があった
// return 2 : 重さが25kgを超えた
int enterWeight(float *weight){
  
  // 入力された文字列を格納
  char buffer[8];
  // float型に変換できなかった文字へのポインタを格納
  char *endptr;
  // 入力をfloat型に変換した数値を格納
  float num = 0;

  // 入力の先頭から、改行を除いた7文字のみを受け入れる
  // 測定器が1g単位なら、「25.000」の6文字で十分
  // 改行を除かないと、改行のみの入力も受け入れてしまう
  // アルファベットなどの無効な入力を検知できるよう、受け入れられる文字数に余裕を持たせている
  // 1文字以上読み取れたときは1を返すので、1以外が返されたら無効な入力として1を返して異常終了
  if(scanf("%7[^\n]%*[^\n]", buffer) != 1) {
    // 次のscanf()のために、残った改行を読み取って捨てる
    // 改行を削除しておかないと無限ループになる
    scanf("%*c");
    puts("何も入力されていません。測定した重さを入力してください。");
    return 1;
  }
  // 次のscanf()のために、残った改行を読み取って捨てる
  scanf("%*c");

  // 7文字以上入力された場合
  // 入力し直すよう促すメッセージを表示
  // 1を返して異常終了
  if(strlen(buffer) == 7) {
    puts("入力が長すぎます。入力し直してください。");
    return 1;
  }

  // 入力をfloat型に変換
  // &endptrには、変換できなかった文字へのポインタを格納
  num = strtof(buffer, &endptr);

  // 変換できない文字があった場合
  // 正しく入力するよう促すメッセージを表示
  // 1を返して異常終了
  if(*endptr != '\0') {
    puts("無効な文字が含まれています。測定した重さを数字のみで入力してください。");
    return 1;
  }

  // 0以下の数値が入力された場合
  // 0より大きい数値を入力するよう促すメッセージを表示
  // 1を返して異常終了
  if(num <= 0) {
    puts("無効な入力です。0より大きい重さを入力してください。");
    return 1;
  }

  *weight = num;
  
  // 重さが25kgを超えていた場合
  // 取り扱えないことを伝えるメッセージを表示
  // 2を返して異常終了
  if(*weight > 25) {
    puts("重さが25kgを超えているので、取り扱うことができません。");
    return 2;
  }

  return 0;
}
