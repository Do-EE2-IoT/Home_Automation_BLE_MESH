import 'package:aithings/core/core.src.dart';

class LoginRepository extends BaseRepository {
  LoginRepository(super.controller);
  Future<BaseResponseBE<LoginResponseModel>> login(
      String username, String password) async {
    final response = await baseSendRequest(
      AppUrl.urlLogin,
      EnumRequestMethod.post,
      jsonMap:
          LoginRequestModel(username: username, password: password).toJson(),
    );
    return BaseResponseBE<LoginResponseModel>.fromJson(
      response,
      func: (x) => LoginResponseModel.fromJson(x),
    );
  }
}
