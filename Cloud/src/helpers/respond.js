// responseUtil.js

/**
 * Tạo một response object theo chuẩn baseResponse
 * @param {any} data - Dữ liệu cần trả về (nếu là chuỗi thì giữ nguyên, nếu không thì chuyển thành JSON)
 * @param {string} code - Mã code của response
 * @param {string|null} [errorMessage=null] - Thông báo lỗi nếu có
 * @returns {Object} - Đối tượng response
 */
function baseResponse(data, code, errorMessage = null) {
  return {
    data: data, // Kiểm tra kiểu dữ liệu của data
    code: code.toString(), //code:
    errorMessage: errorMessage, // message: ""
  };
}

export default baseResponse;
