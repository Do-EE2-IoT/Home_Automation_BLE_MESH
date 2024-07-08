const ErrorCode = {
    100: { status: "Continue" },
    101: { status: "Switching Protocols	" },
    103: { status: "Early Hints	" },
    200: { status: "OK	" },
    201: { status: "Created	" },
    202: { status: "Accepted	" },
    203: { status: "Non - Authoritative Information	" },
    204: { status: "No Content	" },
    205: { status: "Reset Content	" },
    206: { status: "Partial Content	" },
    300: { status: "Multiple Choices	" },
    301: { status: "Moved Permanently	" },
    302: { status: "Found	" },
    303: { status: "See Other	" },
    304: { status: "Not Modified	" },
    307: { status: "Temporary Redirect	" },
    308: { status: "Permanent Redirect	" },
    400: { status: "Bad Request	" },
    401: { status: "Unauthorized	" },
    402: { status: "Payment Required	" },
    403: { status: "Forbidden	" },
    404: { status: "Not Found	" },
    405: { status: "Method Not Allowed	" },
    406: { status: "Not Acceptable	" },
    407: { status: "Proxy Authentication Required	" },
    408: { status: "Request Timeout	" },
    409: { status: "Conflict	" },
    410: { status: "Gone	" },
    411: { status: "Length Required	" },
    412: { status: "Precondition Failed	" },
    413: { status: "Request Too Large	" },
    414: { status: "Request - URI Too Long	" },
    415: { status: "Unsupported Media Type	" },
    416: { status: "Range Not Satisfiable	" },
    417: { status: "Expectation Failed	" },
    500: { status: "Internal Server Error	" },
    501: { status: "Not Implemented	" },
    502: { status: "Bad Gateway	" },
    503: { status: "Service Unavailable" },
    504: { status: "Gateway Timeout	" },
    505: { status: "HTTP Version Not Supported	" },
    511: { status: "Network Authentication Required" }
}

class ErrorResponse extends Error {
    constructor(message, code) {
        super(message)
        this.code = code
    }
}


export default ErrorResponse