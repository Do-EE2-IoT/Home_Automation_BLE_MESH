import '../../../lib.dart';

class BaseResponseBEList<T> {
  BaseResponseBEList({
    required this.result,
    required this.code,
    required this.errorMessage,
  });

  final Result<T>? result;
  final String code;
  final String errorMessage;

  bool get success => code == AppConst.codeResultSuccess;

  bool get failture => code != AppConst.codeResultSuccess;

  factory BaseResponseBEList.fromJson(
      Map<String, dynamic> json, T Function(dynamic x) func) {
    return BaseResponseBEList(
      result: json["result"] == null
          ? null
          : Result<T>.fromJson(json["result"], func),
      code: json["code"] ?? "",
      errorMessage: json["errorMessage"] ?? "",
    );
  }

  Map<String, dynamic> toJson(Function(dynamic x) func) => {
        "result": result?.toJson((x) => func(x)),
        "code": code,
        "errorMessage": errorMessage,
      };
}

class Result<T> {
  Result({
    required this.data,
    required this.total,
    required this.totalUnread,
  });
  final List<T> data;
  final int total;
  final int totalUnread;

  factory Result.fromJson(
      Map<String, dynamic> json, T Function(dynamic x) func) {
    return Result(
      data: json["data"] == null
          ? []
          : List<T>.from(json["data"].map((x) => func(x))),
      total: json["total"] ?? 0,
      totalUnread: json["totalUnread"] ?? 0,
    );
  }

  Map<String, dynamic> toJson(Function(dynamic x) func) => {
        "data": List<dynamic>.from(data.map((x) => func(x))),
        "total": total,
        "totalUnread": totalUnread,
      };
}
