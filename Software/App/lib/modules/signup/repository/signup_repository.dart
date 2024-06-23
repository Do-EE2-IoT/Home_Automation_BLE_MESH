import '../../../lib.dart';

class SignupRepository extends BaseRepository {
  SignupRepository(super.controller);

  Future<BaseResponseBE> register(SignupReqModel model) async {
    final response = await baseSendRequest(
        AppUrl.urlSignup, EnumRequestMethod.post,
        jsonMap: model.toJson());
    return BaseResponseBE.fromJson(response);
  }
}
