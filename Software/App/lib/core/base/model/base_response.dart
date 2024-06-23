
import '../../core.src.dart';

class BaseResponse<T> {
  BaseResponse({
    required this.data,
    required this.code,
    required this.errorMessage,
  });

  final T data;
  final String code;
  final String errorMessage;

  bool get success => code == AppConst.codeResultSuccess;

  bool get failture => code != AppConst.codeResultSuccess;

  Map<String, dynamic> toJson() => {
        "data": data,
        "code": code,
        "errorMessage": errorMessage,
      };

  factory BaseResponse.fromJson(Map<String, dynamic> json) {
    return BaseResponse(
      data: json["data"],
      code: json["code"] ?? "",
      errorMessage: json["errorMessage"] ?? "",
    );
  }
}
