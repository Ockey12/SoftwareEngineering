#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void enterCool(int*);
int enterSize(float*, int);
int enterWeight(float*, int);
int enterStatus(float*, float*, int);
int enterCharge(float, float, int);
void enterContinue(int*);

int main(){

  // サイズの合計
  float size = 0;
  // 重さ
  float weight = 0;
  // 関数の返り値を格納
  int errorFlag = 0;
  // クール便なら1、通常の荷物なら0になるフラグ
  int coolFlag = 0;
  // 続けて他の荷物を入力するなら1、終了するなら0になるフラグ
  int continueFlag = 1;
  // 個別の配送料金
  int charge = 0;
  // 合計の配送料金
  int sumCharge = 0;

  puts("");

  while(continueFlag){

    // クール便かどうかを尋ねる。
    enterCool(&coolFlag);

    printf("サイズを入力してください。\n");

    // サイズを決定
    errorFlag = enterStatus(&size, &weight, coolFlag);

    if(errorFlag != 1){

      // サイズから送料を決定
      charge = enterCharge(size, weight, coolFlag);

      printf("この荷物の配送料金は%d円です。\n", charge);

      sumCharge += charge;
    }

    enterContinue(&continueFlag);
    puts("");

    size = 0;
    weight = 0;
  }

  printf("合計の配送料金は%d円です。\n\n", sumCharge);

  return 0;
}


// クール便かどうかを尋ねて変数coolFlagを決定
void enterCool(int *coolFlag){

  // 入力された文字列を格納
  char buffer[2];
  // int型に変換できなかった文字へのポインタを格納
  char *endptr;
  // 入力をfloat型に変換した数値を格納
  long num = 0;

  while(1) {
    printf("クール便なら1を、通常の配送なら0を入力してください:");

    if(scanf("%2[^\n]%*[^\n]", buffer) != 1) {
      scanf("%*c");
      puts("何も入力されていません。入力し直してください。");
      continue;
    }

    scanf("%*c");

    // 入力が1文字より多い場合
    if(strlen(buffer) == 2) {
      puts("入力が長すぎます。入力し直してください。");
      continue;
    }

    // 入力された文字列をint型に変換
    num = strtol(buffer, &endptr, 10);

    // 変換できない文字があった場合
    if(*endptr != '\0') {
      puts("無効な文字が含まれています。入力し直してください。");
      continue;
    }

    // 0か1以外の数字が入力されたとき
    if( (num != 0) && (num != 1) ) {
      puts("無効な入力です。入力し直してください。");
      continue;
    }

    break;
  }

  *coolFlag = (int)num;

}


// 縦・横・高さのいずれかの入力とエラー処理、合計の加算を行う
// return 0 : 正常に終了
// return 1 : 無効な入力があった
// return 2 : sizeの合計が160を超えた
int enterSize(float *size, int coolFlag){
  
  // 入力された文字列を格納
  char buffer[6];
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
  
  // 縦・横・高さの合計が取り扱えないサイズの場合
  // 取り扱えないことを伝えるメッセージを表示
  // 2を返して異常終了
  if(coolFlag == 1){
    // クール便の場合

    if(*size > 120) {
      puts("箱の3辺の長さの合計が120cmを超えているので、クール便で取り扱うことができません。");
      return 2;
    }
  } else {
    // 通常の配送の場合

    if(*size > 160) {
      puts("箱の3辺の長さの合計が160cmを超えているので、取り扱うことができません。");
      return 2;
    }
  }
  
  return 0;
}


// 重さの入力とエラー処理を行う
// return 0 : 正常に終了
// return 1 : 無効な入力があった
// return 2 : 重さが25kgを超えた
int enterWeight(float *weight, int coolFlag){
  
  // 入力された文字列を格納
  char buffer[7];
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
  
  // 取り扱えない重さの場合
  // 取り扱えないことを伝えるメッセージを表示
  // 2を返して異常終了
  if(coolFlag == 1){
    // クール便の場合

    if(*weight > 15) {
      puts("重さが15kgを超えているので、クール便で取り扱うことができません。");
      return 2;
    }
  } else {
    // 通常の配送の場合

    if(*weight > 25) {
      puts("重さが25kgを超えているので、取り扱うことができません。");
      return 2;
    }
  }
  
  return 0;
}


// 縦、横、高さ、重さの入力を行う
// 荷物のサイズが取り扱えないもののとき1を返して異常終了する
int enterStatus(float *size, float *weight, int coolFlag){

  // 関数の返り値を格納
  int errorFlag = 0;

  // 縦を入力
  // 正しく入力されるまで繰り返す
  while(1) {
    printf("縦(cm)  :");
    errorFlag = enterSize(size, coolFlag);
    if(errorFlag == 1) {
      // 1は無効な入力なので、このループをスキップして次のループで再入力
      continue;
    } else if(errorFlag == 2) {
      // 2は取り扱えないサイズなので1を返して異常終了
      return 1;
    } else {
      // 正しく入力されているのでループを抜ける
      break;
    }
  }

  // 横を入力
  // 正しく入力されるまで繰り返す
  while(1) {
    printf("横(cm)  :");
    errorFlag = enterSize(size, coolFlag);
    if(errorFlag == 1) {
      continue;
    } else if(errorFlag == 2) {
      return 1;
    } else {
      break;
    }
  }

  // 高さを入力
  // 正しく入力されるまで繰り返す
  while(1) {
    printf("高さ(cm):");
    errorFlag = enterSize(size, coolFlag);
    if(errorFlag == 1) {
      continue;
    } else if(errorFlag == 2) {
      return 1;
    } else {
      break;
    }
  }

  // 重さを入力
  // 正しく入力されるまで繰り返す
  while(1) {
    printf("重さ(kg):");
    errorFlag = enterWeight(weight, coolFlag);
    if(errorFlag == 1) {
      continue;
    } else if(errorFlag == 2) {
      return 1;
    } else {
      break;
    }
  }

  return 0;
}


// サイズとクール便のフラグから送料を返す
int enterCharge(float size, float weight, int coolFlag){

  int sizeCharge;
  int weightCharge;

  if(coolFlag == 1){
    // クール便の場合
    
    // 大きさによる料金を決定
    if(size <= 60) {
      sizeCharge = 1260;
    } else if(size <= 80) {
      sizeCharge = 1480;
    } else if(size <= 100) {
      sizeCharge = 1830;
    } else {
      sizeCharge = 2380;
    }

    // 重さによる料金を決定
    if(weight <= 2) {
      weightCharge = 1260;
    } else if(weight <= 5) {
      weightCharge = 1480;
    } else if(weight <= 10) {
      weightCharge = 1830;
    } else {
      weightCharge = 2380;
    }

  } else {
    // 通常の配送の場合

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
  }

  // 高い方の料金を返す
  if(sizeCharge > weightCharge) {
    return sizeCharge;
  } else {
    return weightCharge;
  }

}


// 続けて他の荷物を入力するか尋ねてcontinueFlagを決定
void enterContinue(int *continueFlag){

  // 入力された文字列を格納
  char buffer[2];
  // int型に変換できなかった文字へのポインタを格納
  char *endptr;
  // 入力をfloat型に変換した数値を格納
  long num = 0;

  while(1) {
    printf("続けて他の荷物を入力するなら1を、終了するなら0を入力してください:");

    if(scanf("%2[^\n]%*[^\n]", buffer) != 1) {
      scanf("%*c");
      puts("何も入力されていません。入力し直してください。");
      continue;
    }

    scanf("%*c");

    // 入力が1文字より多い場合
    if(strlen(buffer) == 2) {
      puts("入力が長すぎます。入力し直してください。");
      continue;
    }

    // 入力された文字列をint型に変換
    num = strtol(buffer, &endptr, 10);

    // 変換できない文字があった場合
    if(*endptr != '\0') {
      puts("無効な文字が含まれています。入力し直してください。");
      continue;
    }

    // 0か1以外の数字が入力されたとき
    if( (num != 0) && (num != 1) ) {
      puts("無効な入力です。入力し直してください。");
      continue;
    }

    break;
  }

  *continueFlag = (int)num;
}
