
import '../../../lib.dart';

class BaseResponseBE<T> {
  BaseResponseBE({
    required this.code,
    required this.result,
    required this.errorMessage,
  });

  final T? result;
  final String code;
  final String errorMessage;

  bool get success => code == AppConst.codeResultSuccess;

  bool get failture => code != AppConst.codeResultSuccess;

  factory BaseResponseBE.fromJson(
    Map<String, dynamic> json, {
    T Function(Map<String, dynamic> x)? func,
  }) {
    T? convertObject() => func != null ? func(json["result"]) : json["result"];
    return BaseResponseBE<T>(
      errorMessage: json["errorMessage"] ?? '',
      code: json["code"] ?? "",
      result: json["result"] != null ? convertObject() : null,
    );
  }

  Map<String, dynamic> toJson() => {
        "errorMessage": errorMessage,
        "code": code,
        "result": result,
      };
}
