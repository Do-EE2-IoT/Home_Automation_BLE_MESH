import 'dart:developer';

const vnd = "đ";
const vndUnit = "đồng";

const List<String> _numbericChar = [
  " không",
  " một",
  " hai",
  " ba",
  " bốn",
  " năm",
  " sáu",
  " bảy",
  " tám",
  " chín"
];

const List<String> _moneyChar = [
  "",
  " nghìn",
  " triệu",
  " tỷ",
  " nghìn tỷ",
  " triệu tỷ"
];

const List<String> _moneyPrefixChar = [
  "",
  " nghìn",
  " triệu",
  " tỷ",
  " nghìn",
  " triệu tỷ"
];
//case has billion and thousandbillion.

String numberToWords(int soTien) {
  String str1 = "";
  List<int> numArray = List<int>.filled(6, 0);

  if (soTien < 0) return "Số tiền âm !";
  if (soTien == 0) return "Không đồng !";
  int num1 = soTien <= 0 ? -soTien : soTien;
  if (soTien > 8999999999999999) {
    soTien = 0;
    return "";
  }
  numArray[5] = num1 ~/ 1000000000000000;
  var num2 = num1 - numArray[5] * 1000000000000000;
  numArray[4] = num2 ~/ 1000000000000;
  var num3 = num2 - numArray[4] * 1000000000000;
  numArray[3] = num3 ~/ 1000000000;
  var num4 = num3 - numArray[3] * 1000000000;
  numArray[2] = num4 ~/ 1000000;
  numArray[1] = num4 % 1000000 ~/ 1000;
  numArray[0] = num4 % 1000;
  int num5 = numArray[5] <= 0
      ? (numArray[4] <= 0
          ? (numArray[3] <= 0
              ? (numArray[2] <= 0 ? (numArray[1] <= 0 ? 0 : 1) : 2)
              : 3)
          : 4)
      : 5;
  for (int index = num5; index >= 0; --index) {
    bool isDoc0 = false;
    if (numArray[index].toString().length < 3 && index < num5) {
      isDoc0 = true;
    }
    String str2 = _read3Char(numArray[index], isDoc0);
    // ignore: use_string_buffers
    str1 += str2;
    if (numArray[index] != 0 && numArray[4] > 0 && numArray[3] > 0) {
      str1 += _moneyPrefixChar[index];
    } else if (numArray[index] != 0) {
      str1 += _moneyChar[index];
    }
    if (index > 0 && str2.isNotEmpty) str1 = str1;
  }
  try {
    if (str1.substring(1, str1.length - 1) == ",") {
      str1 = str1.substring(0, str1.length - 1);
    }
  } catch (ex) {
    log("$ex");
  }
  String str3 = str1.trim();
  return "${str3.substring(0, 1).toUpperCase()}${str3.substring(1)} đồng";
}

// String _addZero(String str) {
//   if (str.length == 2)
//     str = "0" + str;
//   else if (str.length == 1) str = "00" + str;
//   return str;
// }

String _read3Char(int baso, bool isDoc0) {
  String str = "";
  int index1 = baso ~/ 100;
  int index2 = baso % 100 ~/ 10;
  int index3 = baso % 10;
  if (index1 == 0 && index2 == 0 && index3 == 0) return "";
  if (index1 != 0 || isDoc0) {
    str = "$str${_numbericChar[index1.toInt()]} trăm";
    if (index2 == 0 && index3 != 0) str += " linh";
  }
  if (index2 != 0 && index2 != 1) {
    str = "$str${_numbericChar[index2.toInt()]} mươi";
    if (index2 == 0 && index3 != 0) str += " linh";
  }
  if (index2 == 1) str += " mười";
  switch (index3.toInt()) {
    case 1:
      str = index2 == 0 || index2 == 1
          ? str + _numbericChar[index3.toInt()]
          : "$str mốt";
      return str;
    case 5:
      str = index2 != 0 ? "$str lăm" : str + _numbericChar[index3.toInt()];
      return str;
    case 0:
      return str;
    default:
      str += _numbericChar[index3.toInt()];
      return str;
  }
}
