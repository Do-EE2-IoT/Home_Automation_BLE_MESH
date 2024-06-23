import 'package:dio/dio.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';

import '../../core.src.dart';

class BaseGetxController extends GetxController {
  RxBool isShowLoading = false.obs;
  String errorContent = '';
  RxBool isRemember = false.obs;
  BaseRequest baseRequestController = Get.find<BaseRequest>();

  ///1 CancelToken để huỷ 1 request.
  ///1 màn hình gắn với 1 controller, 1 controller có nhiều requests khi huỷ 1 màn hình => huỷ toàn bộ requests `INCOMPLETED` tại màn hình đó.
  List<CancelToken> cancelTokens = [];

  /// Sử dụng một số màn bắt buộc sử dụng loading overlay
  RxBool isLoadingOverlay = false.obs;

  void showLoading() {
    isShowLoading.value = true;
  }

  void hideLoading() {
    isShowLoading.value = false;
  }

  void showLoadingOverlay() {
    isLoadingOverlay.value = true;
  }

  void hideLoadingOverlay() {
    isLoadingOverlay.value = false;
  }

  void addCancelToken(CancelToken cancelToken) {
    cancelTokens.add(cancelToken);
  }

  void cancelRequest(CancelToken cancelToken) {
    if (!cancelToken.isCancelled) {
      cancelToken.cancel('Cancel when close controller!!!');
    }
  }

  Future<void> showSnackBar<T>(
    String message, {
    Duration duration = const Duration(seconds: 2),
    Widget? mainButton,
    Color backgroundColor = AppColors.basicWhite,
  }) async {
    Get.showSnackbar(GetSnackBar(
      backgroundColor: AppColors.basicWhite,
      messageText: Text(
        message.tr,
        style: Get.textTheme.bodyMedium,
      ),
      message: message.tr,
      mainButton: Row(
        children: [
          if (mainButton != null) mainButton,
          IconButton(
              onPressed: () => Get.back(), icon: const Icon(Icons.close)),
        ],
      ),
      duration: message.length > 100 ? 5.seconds : duration,
    ));
  }

  // void _setOnErrorListener() {
  //   baseRequestController.setOnErrorListener((error) async {
  //     errorContent = AppStr.errorConnectFailedStr.tr;
  //
  //     if (error is DioException) {
  //       //Nếu server có trả về message thì hiển thị
  //       if (error.response?.data != null &&
  //           error.response!.data is Map &&
  //           error.response!.data["Message"] != null) {
  //         // trường hợp tài khoản không hợp lệ thì thông báo khách hàng đăng nhập lại
  //         /*  if (error.response!.data['ErrorCode'] == '177') {
  //           errorContent = AppStr.error177.tr;
  //         } else {
  //           errorContent = error.response!.data['Message'];
  //         }*/
  //         errorContent = error.response!.data['Message'];
  //       } else {
  //         switch (error.type) {
  //           case DioExceptionType.connectionTimeout:
  //           case DioExceptionType.sendTimeout:
  //           case DioExceptionType.receiveTimeout:
  //             errorContent = AppStr.errorConnectTimeOut.tr;
  //             break;
  //           case DioExceptionType.cancel:
  //             // không hiện thông báo khi huỷ request
  //             errorContent = '';
  //             break;
  //           case DioExceptionType.badResponse:
  //             switch (error.response!.statusCode) {
  //
  //               case AppConst.error400:
  //                 errorContent = AppStr.error400.tr;
  //                 break;
  //               case AppConst.error401:
  //                 errorContent = AppStr.error401.tr;
  //                 break;
  //               case AppConst.error404:
  //                 errorContent = AppStr.error404.tr;
  //                 break;
  //               case AppConst.error500:
  //                 errorContent = AppStr.errorInternalServer.tr;
  //                 break;
  //               case AppConst.error502:
  //                 errorContent = AppStr.error502.tr;
  //                 break;
  //               case AppConst.error503:
  //                 errorContent = AppStr.error503.tr;
  //                 break;
  //               default:
  //                 errorContent = AppStr.errorInternalServer.tr;
  //             }
  //             break;
  //           default:
  //             errorContent = AppStr.errorConnectFailedStr.tr;
  //         }
  //         /*  // check lỗi khi tải pdf
  //        if (error.response?.data != null &&
  //             error.response?.data is List<int>) {
  //           var result = utf8.decode(error.response?.data);
  //           var err = jsonDecode(result);
  //           if (err is Map) {
  //             errorContent = err['Message'];
  //           }
  //         }*/
  //       }
  //       await FirebaseCloud.addLogError(error, errorContent);
  //     }
  //
  //     isShowLoading.value = false;
  //     isLoadingOverlay.value = false;
  //     // if (errorContent.isNotEmpty) showSnackBar(errorContent);
  //     if (errorContent.isNotEmpty) {
  //       ShowPopup.showErrorMessage(errorContent);
  //     }
  //   });
  // }

  @override
  void onClose() {
    for (var cancelToken in cancelTokens) {
      cancelRequest(cancelToken);
    }
    super.onClose();
  }

  @override
  void onReady() {
    // _setOnErrorListener();
    super.onReady();
  }
}
