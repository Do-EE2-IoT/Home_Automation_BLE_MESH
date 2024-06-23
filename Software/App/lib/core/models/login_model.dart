class LoginResponseModel {
  LoginResponseModel({
    required this.data,
    required this.code,
    required this.errorMessage,
  });

  final String data;
  final int code;
  final String errorMessage;

  factory LoginResponseModel.fromJson(Map<String, dynamic> json) {
    return LoginResponseModel(
      data: json["data"] ?? "",
      code: json["code"] ?? 0,
      errorMessage: json["errorMessage"] ?? "",
    );
  }

  Map<String, dynamic> toJson() => {
        "data": data,
        "code": code,
        "errorMessage": errorMessage,
      };
}

class LoginRequestModel {
    LoginRequestModel({
        required this.username,
        required this.password,
    });

    final String username;
    final String password;

    Map<String, dynamic> toJson() => {
        "username": username,
        "password": password,
    };

}
