#pragma once

#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <vector>

namespace restcpp
{

    /**
     * @brief Thread count to be used for thread pool by server
     * 
     */
    static constexpr int THREAD_COUNT = 3;

    /**
     * @brief HTTP method types
     * 
     */
    enum class METHOD {
        GET,POST,PUT,PATCH,DEL,HEAD,OPTIONS,CONNECT,TRACE,INVALID
    };

    /**
     * @brief Struct to keep HTTP versions
     * 
     */
    typedef struct {
        short majorVersion,minorVersion;
    } HTTP_VERSION;


    /**
     * @brief HTTP Protocols
     * 
     */
    enum class HTTP_PROTOCOL {
        HTTP1_0,HTTP1_1,HTTP2_0,HTTP3_0,INVALID
    };

    typedef uint64_t SOCKET;

    enum class TRANSFER_ENCODING {
        CHUNKED,IDENTITY,INVALID
    };

    enum class CONTENT_ENCODING {
        GZIP,DEFLATE,INVALID
    };


    /**
     * @brief Function to serialized method to string
     * 
     * @param method 
     * @return std::string 
     */
    inline std::string gMethodToStr(const METHOD& method)
    {
        switch(method)
        {
            case METHOD::GET:       return "GET";
            case METHOD::POST:      return "POST";
            case METHOD::PUT:       return "PUT";
            case METHOD::PATCH:     return "PATCH";
            case METHOD::DEL:    return "DELETE";
            case METHOD::HEAD:      return "HEAD";
            case METHOD::OPTIONS:   return "OPTIONS";
        }

        return "";
    }


        inline std::vector<std::string> g_splitByChar(std::string to_split, char delimiter) {
            std::vector<std::string> tokens;
            std::string token;

            for (const auto& c: to_split) 
            {
                if (c != delimiter)
                    token += c;
                else 
                {
                    if (token.length()) 
                        tokens.push_back(token);
                    token.clear();
                }
            }

            if (token.length()) 
                tokens.push_back(token);
            return tokens;
        }

        inline std::vector<std::string> g_splitByStr(std::string str,std::string delimeter)
        {
            std::vector<std::string> splittedStrings = {};
            size_t pos = 0;

            while ((pos = str.find(delimeter)) != std::string::npos)
            {
                std::string token = str.substr(0, pos);
                if (token.length() > 0)
                    splittedStrings.push_back(token);
                str.erase(0, pos + delimeter.length());
            }

            if (str.length() > 0)
                splittedStrings.push_back(str);
            return splittedStrings;
        }

        inline std::string g_decodeUri(const char* in) 
        {
            std::string res;
            static const char tbl[256] = {
                -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
                0, 1, 2, 3, 4, 5, 6, 7,  8, 9,-1,-1,-1,-1,-1,-1,
                -1,10,11,12,13,14,15,-1, -1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
                -1,10,11,12,13,14,15,-1, -1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
                -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1
            };
            char c, v1, v2;
            if(in != NULL) {
                while((c=*in++) != '\0') {
                    if(c == '%') {
                        if((v1=tbl[(unsigned char)*in++])<0 || 
                        (v2=tbl[(unsigned char)*in++])<0) {
                            return res;
                        }
                        c = (v1<<4)|v2;
                    }
                    res += c;
                }
            }
            return res;
        }


    /**
     * @brief Get description of status code as string
     * 
     * @param statusCode 
     * @return std::string 
     */
    inline std::string gGetStatusDescription(short statusCode)
    {
        std::string res;
        switch(statusCode)
        {
            case 100:	res = "Continue";break;
            case 101:	res = "Switching protocols";break;
            case 102:	res = "Processing";break;
            case 103:	res = "Early Hints	 ";break;
            case 200:	res = "OK";break;
            case 201:	res = "Created";break;
            case 202:	res = "Accepted";break;
            case 203:   res = "Non-Authoritative Information";break;
            case 204:	res = "No Content";break;
            case 205:	res = "Reset Content";break;
            case 206:	res = "Partial Content";break;
            case 207:	res = "Multi-Status";break;
            case 208:	res = "Already Reported";break;
            case 226:	res = "IM Used";break;
            case 300:	res = "Multiple Choices";break;
            case 301:	res = "Moved Permanently";break;
            case 302:	res = "Found";break;
            case 303:	res = "See Other";break;
            case 304:	res = "Not Modified";break;
            case 305:	res = "Use Proxy";break;
            case 306:	res = "Switch Proxy";break;
            case 307:	res = "Temporary Redirect";break;
            case 308:	res = "Permanent Redirect	 	 ";break;
            case 400:	res = "Bad Request";break;
            case 401:	res = "Unauthorized";break;
            case 402:	res = "Payment Required";break;
            case 403:	res = "Forbidden";break;
            case 404:	res = "Not Found";break;
            case 405:	res = "Method Not Allowed";break;
            case 406:	res = "Not Acceptable";break;
            case 407:	res = "Proxy Authentication Required";break;
            case 408:	res = "Request Timeout";break;
            case 409:	res = "Conflict";break;
            case 410:	res = "Gone";break;
            case 411:	res = "Length Required";break;
            case 412:	res = "Precondition Failed";break;
            case 413:	res = "Payload Too Large";break;
            case 414:	res = "URI Too Long";break;
            case 415:	res = "Unsupported Media Type";break;
            case 416:	res = "Range Not Satisfiable";break;
            case 417:	res = "Expectation Failed";break;
            case 418:	res = "I'm a Teapot";break;
            case 421:	res = "Misdirected Request";break;
            case 422:	res = "Unprocessable Entity";break;
            case 423:	res = "Locked";break;
            case 424:	res = "Failed Dependency";break;
            case 425:	res = "Too Early";break;
            case 426:	res = "Upgrade Required";break;
            case 428:	res = "Precondition Required";break;
            case 429:	res = "Too Many Requests";break;
            case 431:	res = "Request Header Fields Too Large";break;
            case 451:	res = "Unavailable For Legal Reasons";break;
            case 500:	res = "Internal Server Error";break;
            case 501:	res = "Not Implemented";break;
            case 502:	res = "Bad Gateway";break;
            case 503:	res = "Service Unavailable";break;
            case 504:	res = "Gateway Timeout";break;
            case 505:	res = "HTTP Version Not Supported";break;
            case 506:	res = "Variant Also Negotiates";break;
            case 507:	res = "Insufficient Storage";break;
            case 508:	res = "Loop Detected";break;
            case 510:	res = "Not Extended";break;
            case 511:	res = "Network Authentication Required";break;
        }

        return res;
    }



    typedef unsigned char byte;

    /**
     * @brief Map to store MIME types and descriptions
     * 
     */
    const static std::unordered_map<std::string,std::string> gMIMETable = {
        {"3dm", "x-world/x-3dmf"},
        {"3dmf","x-world/x-3dmf"},
        {"a","application/octet-stream"},
        {"aab","application/x-authorware-bin"},
        {"aam","application/x-authorware-map"},
        {"aas","application/x-authorware-seg"},
        {"abc","text/vnd.abc"},
        {"acgi","text/html"},
        {"afl","video/animaflex"},
        {"ai","application/postscript"},
        {"aif","audio/aiff"},
        {"aif","audio/x-aiff"},
        {"aifc","audio/aiff"},
        {"aifc","audio/x-aiff"},
        {"aiff","audio/aiff"},
        {"aiff","audio/x-aiff"},
        {"aim","application/x-aim"},
        {"aip","text/x-audiosoft-intra"},
        {"ani","application/x-navi-animation"},
        {"aos","application/x-nokia-9000-communicator-add-on-software"},
        {"aps","application/mime"},
        {"arc","application/octet-stream"},
        {"arj","application/arj"},
        {"arj","application/octet-stream"},
        {"art","image/x-jg"},
        {"asf","video/x-ms-asf"},
        {"asm","text/x-asm"},
        {"asp","text/asp"},
        {"asx","application/x-mplayer2"},
        {"asx","video/x-ms-asf"},
        {"asx","video/x-ms-asf-plugin"},
        {"au","audio/basic"},
        {"au","audio/x-au"},
        {"avi","application/x-troff-msvideo"},
        {"avi","video/avi"},
        {"avi","video/msvideo"},
        {"avi","video/x-msvideo"},
        {"avs","video/avs-video"},
        {"bcpio","application/x-bcpio"},
        {"bin","application/mac-binary"},
        {"bin","application/macbinary"},
        {"bin","application/octet-stream"},
        {"bin","application/x-binary"},
        {"bin","application/x-macbinary"},
        {"bm","image/bmp"},
        {"bmp","image/bmp"},
        {"bmp","image/x-windows-bmp"},
        {"boo","application/book"},
        {"book","application/book"},
        {"boz","application/x-bzip2"},
        {"bsh","application/x-bsh"},
        {"bz","application/x-bzip"},
        {"bz2","application/x-bzip2"},
        {"c","text/plain"},
        {"c","text/x-c"},
        {"c++","text/plain"},
        {"cat","application/vnd.ms-pki.seccat"},
        {"cc","text/plain"},
        {"cc","text/x-c"},
        {"ccad","application/clariscad"},
        {"cco","application/x-cocoa"},
        {"cdf","application/cdf"},
        {"cdf","application/x-cdf"},
        {"cdf","application/x-netcdf"},
        {"cer","application/pkix-cert"},
        {"cer","application/x-x509-ca-cert"},
        {"cha","application/x-chat"},
        {"chat","application/x-chat"},
        {"class","application/java"},
        {"class","application/java-byte-code"},
        {"class","application/x-java-class"},
        {"com","application/octet-stream"},
        {"com","text/plain"},
        {"conf","text/plain"},
        {"cpio","application/x-cpio"},
        {"cpp","text/x-c"},
        {"cpt","application/mac-compactpro"},
        {"cpt","application/x-compactpro"},
        {"cpt","application/x-cpt"},
        {"crl","application/pkcs-crl"},
        {"crl","application/pkix-crl"},
        {"crt","application/pkix-cert"},
        {"crt","application/x-x509-ca-cert"},
        {"crt","application/x-x509-user-cert"},
        {"csh","application/x-csh"},
        {"csh","text/x-script.csh"},
        {"css","text/css"},
        {"cxx","text/plain"},
        {"dcr","application/x-director"},
        {"deepv","application/x-deepv"},
        {"def","text/plain"},
        {"der","application/x-x509-ca-cert"},
        {"dif","video/x-dv"},
        {"dir","application/x-director"},
        {"dl","video/dl"},
        {"dl","video/x-dl"},
        {"doc","application/msword"},
        {"dot","application/msword"},
        {"dp","application/commonground"},
        {"drw","application/drafting"},
        {"dump","application/octet-stream"},
        {"dv","video/x-dv"},
        {"dvi","application/x-dvi"},
        {"dwf","drawing/x-dwf {old},"},
        {"dwf","model/vnd.dwf"},
        {"dwg","application/acad"},
        {"dwg","image/vnd.dwg"},
        {"dwg","image/x-dwg"},
        {"dxf","application/dxf"},
        {"dxf","image/vnd.dwg"},
        {"dxf","image/x-dwg"},
        {"dxr","application/x-director"},
        {"el","text/x-script.elisp"},
        {"elc","application/x-bytecode.elisp {compiled elisp},"},
        {"elc","application/x-elc"},
        {"env","application/x-envoy"},
        {"eps","application/postscript"},
        {"es","application/x-esrehber"},
        {"etx","text/x-setext"},
        {"evy","application/envoy"},
        {"evy","application/x-envoy"},
        {"exe","application/octet-stream"},
        {"f","text/plain"},
        {"f","text/x-fortran"},
        {"f77","text/x-fortran"},
        {"f90","text/plain"},
        {"f90","text/x-fortran"},
        {"fdf","application/vnd.fdf"},
        {"fif","application/fractals"},
        {"fif","image/fif"},
        {"fli","video/fli"},
        {"fli","video/x-fli"},
        {"flo","image/florian"},
        {"flx","text/vnd.fmi.flexstor"},
        {"fmf","video/x-atomic3d-feature"},
        {"for","text/plain"},
        {"for","text/x-fortran"},
        {"fpx","image/vnd.fpx"},
        {"fpx","image/vnd.net-fpx"},
        {"frl","application/freeloader"},
        {"funk","audio/make"},
        {"g","text/plain"},
        {"g3","image/g3fax"},
        {"gif","image/gif"},
        {"gl","video/gl"},
        {"gl","video/x-gl"},
        {"gsd","audio/x-gsm"},
        {"gsm","audio/x-gsm"},
        {"gsp","application/x-gsp"},
        {"gss","application/x-gss"},
        {"gtar","application/x-gtar"},
        {"gz","application/x-compressed"},
        {"gz","application/x-gzip"},
        {"gzip","application/x-gzip"},
        {"gzip","multipart/x-gzip"},
        {"h","text/plain"},
        {"h","text/x-h"},
        {"hdf","application/x-hdf"},
        {"help","application/x-helpfile"},
        {"hgl","application/vnd.hp-hpgl"},
        {"hh","text/plain"},
        {"hh","text/x-h"},
        {"hlb","text/x-script"},
        {"hlp","application/hlp"},
        {"hlp","application/x-helpfile"},
        {"hlp","application/x-winhelp"},
        {"hpg","application/vnd.hp-hpgl"},
        {"hpgl","application/vnd.hp-hpgl"},
        {"hqx","application/binhex"},
        {"hqx","application/binhex4"},
        {"hqx","application/mac-binhex"},
        {"hqx","application/mac-binhex40"},
        {"hqx","application/x-binhex40"},
        {"hqx","application/x-mac-binhex40"},
        {"hta","application/hta"},
        {"htc","text/x-component"},
        {"htm","text/html"},
        {"html","text/html"},
        {"htmls","text/html"},
        {"htt","text/webviewhtml"},
        {"htx","text/html"},
        {"ice","x-conference/x-cooltalk"},
        {"ico","image/x-icon"},
        {"idc","text/plain"},
        {"ief","image/ief"},
        {"iefs","image/ief"},
        {"iges","application/iges"},
        {"iges","model/iges"},
        {"igs","application/iges"},
        {"igs","model/iges"},
        {"ima","application/x-ima"},
        {"imap","application/x-httpd-imap"},
        {"inf","application/inf"},
        {"ins","application/x-internett-signup"},
        {"ip","application/x-ip2"},
        {"isu","video/x-isvideo"},
        {"it","audio/it"},
        {"iv","application/x-inventor"},
        {"ivr","i-world/i-vrml"},
        {"ivy","application/x-livescreen"},
        {"jam","audio/x-jam"},
        {"jav","text/plain"},
        {"jav","text/x-java-source"},
        {"java","text/plain"},
        {"java","text/x-java-source"},
        {"jcm","application/x-java-commerce"},
        {"jfif","image/jpeg"},
        {"jfif","image/pjpeg"},
        {"jfif-tbnl","image/jpeg"},
        {"jpe","image/jpeg"},
        {"jpe","image/pjpeg"},
        {"jpeg","image/jpeg"},
        {"jpeg","image/pjpeg"},
        {"jpg","image/jpeg"},
        {"jpg","image/pjpeg"},
        {"jps","image/x-jps"},
        {"js","application/x-javascript"},
        {"js","application/javascript"},
        {"js","application/ecmascript"},
        {"js","text/javascript"},
        {"js","text/ecmascript"},
        {"jut","image/jutvision"},
        {"kar","audio/midi"},
        {"kar","music/x-karaoke"},
        {"ksh","application/x-ksh"},
        {"ksh","text/x-script.ksh"},
        {"la","audio/nspaudio"},
        {"la","audio/x-nspaudio"},
        {"lam","audio/x-liveaudio"},
        {"latex","application/x-latex"},
        {"lha","application/lha"},
        {"lha","application/octet-stream"},
        {"lha","application/x-lha"},
        {"lhx","application/octet-stream"},
        {"list","text/plain"},
        {"lma","audio/nspaudio"},
        {"lma","audio/x-nspaudio"},
        {"log","text/plain"},
        {"lsp","application/x-lisp"},
        {"lsp","text/x-script.lisp"},
        {"lst","text/plain"},
        {"lsx","text/x-la-asf"},
        {"ltx","application/x-latex"},
        {"lzh","application/octet-stream"},
        {"lzh","application/x-lzh"},
        {"lzx","application/lzx"},
        {"lzx","application/octet-stream"},
        {"lzx","application/x-lzx"},
        {"m","text/plain"},
        {"m","text/x-m"},
        {"m1v","video/mpeg"},
        {"m2a","audio/mpeg"},
        {"m2v","video/mpeg"},
        {"m3u","audio/x-mpequrl"},
        {"man","application/x-troff-man"},
        {"map","application/x-navimap"},
        {"mar","text/plain"},
        {"mbd","application/mbedlet"},
        {"mc$","application/x-magic-cap-package-1.0"},
        {"mcd","application/mcad"},
        {"mcd","application/x-mathcad"},
        {"mcf","image/vasa"},
        {"mcf","text/mcf"},
        {"mcp","application/netmc"},
        {"me","application/x-troff-me"},
        {"mht","message/rfc822"},
        {"mhtml","message/rfc822"},
        {"mid","application/x-midi"},
        {"mid","audio/midi"},
        {"mid","audio/x-mid"},
        {"mid","audio/x-midi"},
        {"mid","music/crescendo"},
        {"mid","x-music/x-midi"},
        {"midi","application/x-midi"},
        {"midi","audio/midi"},
        {"midi","audio/x-mid"},
        {"midi","audio/x-midi"},
        {"midi","music/crescendo"},
        {"midi","x-music/x-midi"},
        {"mif","application/x-frame"},
        {"mif","application/x-mif"},
        {"mime","message/rfc822"},
        {"mime","www/mime"},
        {"mjf","audio/x-vnd.audioexplosion.mjuicemediafile"},
        {"mjpg","video/x-motion-jpeg"},
        {"mm","application/base64"},
        {"mm","application/x-meme"},
        {"mme","application/base64"},
        {"mod","audio/mod"},
        {"mod","audio/x-mod"},
        {"moov","video/quicktime"},
        {"mov","video/quicktime"},
        {"movie","video/x-sgi-movie"},
        {"mp2","audio/mpeg"},
        {"mp2","audio/x-mpeg"},
        {"mp2","video/mpeg"},
        {"mp2","video/x-mpeg"},
        {"mp2","video/x-mpeq2a"},
        {"mp3","audio/mpeg3"},
        {"mp3","audio/x-mpeg-3"},
        {"mp3","video/mpeg"},
        {"mp3","video/x-mpeg"},
        {"mp4","video/mp4"},
        {"mpa","audio/mpeg"},
        {"mpa","video/mpeg"},
        {"mpc","application/x-project"},
        {"mpe","video/mpeg"},
        {"mpeg","video/mpeg"},
        {"mpg","audio/mpeg"},
        {"mpg","video/mpeg"},
        {"mpga","audio/mpeg"},
        {"mpp","application/vnd.ms-project"},
        {"mpt","application/x-project"},
        {"mpv","application/x-project"},
        {"mpx","application/x-project"},
        {"mrc","application/marc"},
        {"ms","application/x-troff-ms"},
        {"mv","video/x-sgi-movie"},
        {"my","audio/make"},
        {"mzz","application/x-vnd.audioexplosion.mzz"},
        {"nap","image/naplps"},
        {"naplps","image/naplps"},
        {"nc","application/x-netcdf"},
        {"ncm","application/vnd.nokia.configuration-message"},
        {"nif","image/x-niff"},
        {"niff","image/x-niff"},
        {"nix","application/x-mix-transfer"},
        {"nsc","application/x-conference"},
        {"nvd","application/x-navidoc"},
        {"o","application/octet-stream"},
        {"oda","application/oda"},
        {"omc","application/x-omc"},
        {"omcd","application/x-omcdatamaker"},
        {"omcr","application/x-omcregerator"},
        {"p","text/x-pascal"},
        {"p10","application/pkcs10"},
        {"p10","application/x-pkcs10"},
        {"p12","application/pkcs-12"},
        {"p12","application/x-pkcs12"},
        {"p7a","application/x-pkcs7-signature"},
        {"p7c","application/pkcs7-mime"},
        {"p7c","application/x-pkcs7-mime"},
        {"p7m","application/pkcs7-mime"},
        {"p7m","application/x-pkcs7-mime"},
        {"p7r","application/x-pkcs7-certreqresp"},
        {"p7s","application/pkcs7-signature"},
        {"part","application/pro_eng"},
        {"pas","text/pascal"},
        {"pbm","image/x-portable-bitmap"},
        {"pcl","application/vnd.hp-pcl"},
        {"pcl","application/x-pcl"},
        {"pct","image/x-pict"},
        {"pcx","image/x-pcx"},
        {"pdb","chemical/x-pdb"},
        {"pdf","application/pdf"},
        {"pfunk","audio/make"},
        {"pfunk","audio/make.my.funk"},
        {"pgm","image/x-portable-graymap"},
        {"pgm","image/x-portable-greymap"},
        {"pic","image/pict"},
        {"pict","image/pict"},
        {"pkg","application/x-newton-compatible-pkg"},
        {"pko","application/vnd.ms-pki.pko"},
        {"pl","text/plain"},
        {"pl","text/x-script.perl"},
        {"plx","application/x-pixclscript"},
        {"pm","image/x-xpixmap"},
        {"pm","text/x-script.perl-module"},
        {"pm4","application/x-pagemaker"},
        {"pm5","application/x-pagemaker"},
        {"png","image/png"},
        {"pnm","application/x-portable-anymap"},
        {"pnm","image/x-portable-anymap"},
        {"pot","application/mspowerpoint"},
        {"pot","application/vnd.ms-powerpoint"},
        {"pov","model/x-pov"},
        {"ppa","application/vnd.ms-powerpoint"},
        {"ppm","image/x-portable-pixmap"},
        {"pps","application/mspowerpoint"},
        {"pps","application/vnd.ms-powerpoint"},
        {"ppt","application/mspowerpoint"},
        {"ppt","application/powerpoint"},
        {"ppt","application/vnd.ms-powerpoint"},
        {"ppt","application/x-mspowerpoint"},
        {"ppz","application/mspowerpoint"},
        {"pre","application/x-freelance"},
        {"prt","application/pro_eng"},
        {"ps","application/postscript"},
        {"psd","application/octet-stream"},
        {"pvu","paleovu/x-pv"},
        {"pwz","application/vnd.ms-powerpoint"},
        {"py","text/x-script.phyton"},
        {"pyc","application/x-bytecode.python"},
        {"qcp","audio/vnd.qcelp"},
        {"qd3","x-world/x-3dmf"},
        {"qd3d","x-world/x-3dmf"},
        {"qif","image/x-quicktime"},
        {"qt","video/quicktime"},
        {"qtc","video/x-qtc"},
        {"qti","image/x-quicktime"},
        {"qtif","image/x-quicktime"},
        {"ra","audio/x-pn-realaudio"},
        {"ra","audio/x-pn-realaudio-plugin"},
        {"ra","audio/x-realaudio"},
        {"ram","audio/x-pn-realaudio"},
        {"ras","application/x-cmu-raster"},
        {"ras","image/cmu-raster"},
        {"ras","image/x-cmu-raster"},
        {"rast","image/cmu-raster"},
        {"rexx","text/x-script.rexx"},
        {"rf","image/vnd.rn-realflash"},
        {"rgb","image/x-rgb"},
        {"rm","application/vnd.rn-realmedia"},
        {"rm","audio/x-pn-realaudio"},
        {"rmi","audio/mid"},
        {"rmm","audio/x-pn-realaudio"},
        {"rmp","audio/x-pn-realaudio"},
        {"rmp","audio/x-pn-realaudio-plugin"},
        {"rng","application/ringing-tones"},
        {"rng","application/vnd.nokia.ringing-tone"},
        {"rnx","application/vnd.rn-realplayer"},
        {"roff","application/x-troff"},
        {"rp","image/vnd.rn-realpix"},
        {"rpm","audio/x-pn-realaudio-plugin"},
        {"rt","text/richtext"},
        {"rt","text/vnd.rn-realtext"},
        {"rtf","application/rtf"},
        {"rtf","application/x-rtf"},
        {"rtf","text/richtext"},
        {"rtx","application/rtf"},
        {"rtx","text/richtext"},
        {"rv","video/vnd.rn-realvideo"},
        {"s","text/x-asm"},
        {"s3m","audio/s3m"},
        {"saveme","application/octet-stream"},
        {"sbk","application/x-tbook"},
        {"scm","application/x-lotusscreencam"},
        {"scm","text/x-script.guile"},
        {"scm","text/x-script.scheme"},
        {"scm","video/x-scm"},
        {"sdml","text/plain"},
        {"sdp","application/sdp"},
        {"sdp","application/x-sdp"},
        {"sdr","application/sounder"},
        {"sea","application/sea"},
        {"sea","application/x-sea"},
        {"set","application/set"},
        {"sgm","text/sgml"},
        {"sgm","text/x-sgml"},
        {"sgml","text/sgml"},
        {"sgml","text/x-sgml"},
        {"sh","application/x-bsh"},
        {"sh","application/x-sh"},
        {"sh","application/x-shar"},
        {"sh","text/x-script.sh"},
        {"shar","application/x-bsh"},
        {"shar","application/x-shar"},
        {"shtml","text/html"},
        {"shtml","text/x-server-parsed-html"},
        {"sid","audio/x-psid"},
        {"sit","application/x-sit"},
        {"sit","application/x-stuffit"},
        {"skd","application/x-koan"},
        {"skm","application/x-koan"},
        {"skp","application/x-koan"},
        {"skt","application/x-koan"},
        {"sl","application/x-seelogo"},
        {"smi","application/smil"},
        {"smil","application/smil"},
        {"snd","audio/basic"},
        {"snd","audio/x-adpcm"},
        {"sol","application/solids"},
        {"spc","application/x-pkcs7-certificates"},
        {"spc","text/x-speech"},
        {"spl","application/futuresplash"},
        {"spr","application/x-sprite"},
        {"sprite","application/x-sprite"},
        {"src","application/x-wais-source"},
        {"ssi","text/x-server-parsed-html"},
        {"ssm","application/streamingmedia"},
        {"sst","application/vnd.ms-pki.certstore"},
        {"step","application/step"},
        {"stl","application/sla"},
        {"stl","application/vnd.ms-pki.stl"},
        {"stl","application/x-navistyle"},
        {"stp","application/step"},
        {"sv4cpio","application/x-sv4cpio"},
        {"sv4crc","application/x-sv4crc"},
        {"svf","image/vnd.dwg"},
        {"svf","image/x-dwg"},
        {"svr","application/x-world"},
        {"svr","x-world/x-svr"},
        {"swf","application/x-shockwave-flash"},
        {"t","application/x-troff"},
        {"talk","text/x-speech"},
        {"tar","application/x-tar"},
        {"tbk","application/toolbook"},
        {"tbk","application/x-tbook"},
        {"tcl","application/x-tcl"},
        {"tcl","text/x-script.tcl"},
        {"tcsh","text/x-script.tcsh"},
        {"tex","application/x-tex"},
        {"texi","application/x-texinfo"},
        {"texinfo","application/x-texinfo"},
        {"text","application/plain"},
        {"text","text/plain"},
        {"tgz","application/gnutar"},
        {"tgz","application/x-compressed"},
        {"tif","image/tiff"},
        {"tif","image/x-tiff"},
        {"tiff","image/tiff"},
        {"tiff","image/x-tiff"},
        {"tr","application/x-troff"},
        {"tsi","audio/tsp-audio"},
        {"tsp","application/dsptype"},
        {"tsp","audio/tsplayer"},
        {"tsv","text/tab-separated-values"},
        {"turbot","image/florian"},
        {"txt","text/plain"},
        {"uil","text/x-uil"},
        {"uni","text/uri-list"},
        {"unis","text/uri-list"},
        {"unv","application/i-deas"},
        {"uri","text/uri-list"},
        {"uris","text/uri-list"},
        {"ustar","application/x-ustar"},
        {"ustar","multipart/x-ustar"},
        {"uu","application/octet-stream"},
        {"uu","text/x-uuencode"},
        {"uue","text/x-uuencode"},
        {"vcd","application/x-cdlink"},
        {"vcs","text/x-vcalendar"},
        {"vda","application/vda"},
        {"vdo","video/vdo"},
        {"vew","application/groupwise"},
        {"viv","video/vivo"},
        {"viv","video/vnd.vivo"},
        {"vivo","video/vivo"},
        {"vivo","video/vnd.vivo"},
        {"vmd","application/vocaltec-media-desc"},
        {"vmf","application/vocaltec-media-file"},
        {"voc","audio/voc"},
        {"voc","audio/x-voc"},
        {"vos","video/vosaic"},
        {"vox","audio/voxware"},
        {"vqe","audio/x-twinvq-plugin"},
        {"vqf","audio/x-twinvq"},
        {"vql","audio/x-twinvq-plugin"},
        {"vrml","application/x-vrml"},
        {"vrml","model/vrml"},
        {"vrml","x-world/x-vrml"},
        {"vrt","x-world/x-vrt"},
        {"vsd","application/x-visio"},
        {"vst","application/x-visio"},
        {"vsw","application/x-visio"},
        {"w60","application/wordperfect6.0"},
        {"w61","application/wordperfect6.1"},
        {"w6w","application/msword"},
        {"wav","audio/wav"},
        {"wav","audio/x-wav"},
        {"wb1","application/x-qpro"},
        {"wbmp","image/vnd.wap.wbmp"},
        {"web","application/vnd.xara"},
        {"wiz","application/msword"},
        {"wk1","application/x-123"},
        {"wmf","windows/metafile"},
        {"wml","text/vnd.wap.wml"},
        {"wmlc","application/vnd.wap.wmlc"},
        {"wmls","text/vnd.wap.wmlscript"},
        {"wmlsc","application/vnd.wap.wmlscriptc"},
        {"word","application/msword"},
        {"wp","application/wordperfect"},
        {"wp5","application/wordperfect"},
        {"wp5","application/wordperfect6.0"},
        {"wp6","application/wordperfect"},
        {"wpd","application/wordperfect"},
        {"wpd","application/x-wpwin"},
        {"wq1","application/x-lotus"},
        {"wri","application/mswrite"},
        {"wri","application/x-wri"},
        {"wrl","application/x-world"},
        {"wrl","model/vrml"},
        {"wrl","x-world/x-vrml"},
        {"wrz","model/vrml"},
        {"wrz","x-world/x-vrml"},
        {"wsc","text/scriplet"},
        {"wsrc","application/x-wais-source"},
        {"wtk","application/x-wintalk"},
        {"xbm","image/x-xbitmap"},
        {"xbm","image/x-xbm"},
        {"xbm","image/xbm"},
        {"xdr","video/x-amt-demorun"},
        {"xgz","xgl/drawing"},
        {"xif","image/vnd.xiff"},
        {"xl","application/excel"},
        {"xla","application/excel"},
        {"xla","application/x-excel"},
        {"xla","application/x-msexcel"},
        {"xlb","application/excel"},
        {"xlb","application/vnd.ms-excel"},
        {"xlb","application/x-excel"},
        {"xlc","application/excel"},
        {"xlc","application/vnd.ms-excel"},
        {"xlc","application/x-excel"},
        {"xld","application/excel"},
        {"xld","application/x-excel"},
        {"xlk","application/excel"},
        {"xlk","application/x-excel"},
        {"xll","application/excel"},
        {"xll","application/vnd.ms-excel"},
        {"xll","application/x-excel"},
        {"xlm","application/excel"},
        {"xlm","application/vnd.ms-excel"},
        {"xlm","application/x-excel"},
        {"xls","application/excel"},
        {"xls","application/vnd.ms-excel"},
        {"xls","application/x-excel"},
        {"xls","application/x-msexcel"},
        {"xlt","application/excel"},
        {"xlt","application/x-excel"},
        {"xlv","application/excel"},
        {"xlv","application/x-excel"},
        {"xlw","application/excel"},
        {"xlw","application/vnd.ms-excel"},
        {"xlw","application/x-excel"},
        {"xlw","application/x-msexcel"},
        {"xm","audio/xm"},
        {"xml","application/xml"},
        {"xml","text/xml"},
        {"xmz","xgl/movie"},
        {"xpix","application/x-vnd.ls-xpix"},
        {"xpm","image/x-xpixmap"},
        {"xpm","image/xpm"},
        {"x-png","image/png"},
        {"xsr","video/x-amt-showrun"},
        {"xwd","image/x-xwd"},
        {"xwd","image/x-xwindowdump"},
        {"xyz","chemical/x-pdb"},
        {"z","application/x-compress"},
        {"z","application/x-compressed"},
        {"zip","application/x-compressed"},
        {"zip","application/x-zip-compressed"},
        {"zip","application/zip"},
        {"zip","multipart/x-zip"},
        {"zoo","application/octet-stream"},
        {"zsh","text/x-script.zsh"},
    };


    /* Function to generate UUID */
    inline const std::string generateUUID()
    {
        std::string uuid;
        uuid.reserve(36);
        uuid.append("xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx");
        for (int i = 0; i < 36; i++)
        {
            if (uuid[i] == 'x')
            {
                uuid[i] = '0' + (rand() % 9);
            }
            else if (uuid[i] == 'y')
            {
                uuid[i] = '0' + (rand() % 4);
            }
        }
        return uuid;
    }

        inline std::string getErrorHTML(const int& errorCode)
    {
        std::string html = "<html><head><title>Error " + std::to_string(errorCode) + "</title></head><body><h1>Error " + std::to_string(errorCode) + "</h1><p>";
        switch (errorCode)
        {
        case 400:
            html += "Bad Request";
            break;
        case 401:
            html += "Unauthorized";
            break;
        case 403:
            html += "Forbidden";
            break;
        case 404:
            html += "Not Found";
            break;
        case 405:
            html += "Method Not Allowed";
            break;
        case 406:
            html += "Not Acceptable";
            break;
        case 407:
            html += "Proxy Authentication Required";
            break;
        case 408:
            html += "Request Timeout";
            break;
        case 409:
            html += "Conflict";
            break;
        case 410:
            html += "Gone";
            break;
        case 411:
            html += "Length Required";
            break;
        case 412:
            html += "Precondition Failed";
            break;
        case 413:
            html += "Request Entity Too Large";
            break;
        case 414:
            html += "Request-URI Too Long";
            break;
        case 415:
            html += "Unsupported Media Type";
            break;
        case 416:
            html += "Requested Range Not Satisfiable";
            break;
        case 417:
            html += "Expectation Failed";
            break;
        case 418:
            html += "I'm a teapot";
            break;
        case 420:
            html += "Enhance Your Calm";
            break;
        case 422:
            html += "Unprocessable Entity";
            break;
        case 423:
            html += "Locked";
            break;
        case 424:
            html += "Method Failure";
            break;
        case 425:
            html += "Unordered Collection";
            break;
        case 426:
            html += "Upgrade Required";
            break;
        case 428:
            html += "Precondition Required";
            break;
        case 429:
            html += "Too Many Requests";
            break;
        case 431:
            html += "Request Header Fields Too Large";
            break;
        case 444:
            html += "No Response";
            break;
        case 449:
            html += "Retry With";
            break;
        case 450:
            html += "Blocked by Windows Parental Controls";
            break;
        case 451:
            html += "Unavailable For Legal Reasons";
            break;
        case 500:
            html += "Internal Server Error";
            break;
        case 501:
            html += "Not Implemented";
            break;
        case 502:
            html += "Bad Gateway";
            break;
        case 503:
            html += "Service Unavailable";
            break;
        case 504:
            html += "Gateway Timeout";
            break;
        case 505:
            html += "HTTP Version Not Supported";
            break;
        case 506:
            html += "Variant Also Negotiates";
            break;
        case 507:
            html += "Insufficient Storage";
            break;
        case 508:
            html += "Loop Detected";
            break;
        case 509:
            html += "Bandwidth Limit Exceeded";
            break;
        case 510:
            html += "Not Extended";
            break;
        case 511:
            html += "Network Authentication Required";
            break;
        default:
            html += "Unknown Error";
            break;
        }

        html += "</p><h6>RESTC++</h6></body></html>";

        return html;
    }

}