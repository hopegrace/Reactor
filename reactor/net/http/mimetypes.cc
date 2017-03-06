#include "mimetypes.h"

#include <stdio.h>
#include <string.h>

#include "reactor/base/path.h"
#include "reactor/base/SimpleLogger.h"
#include "reactor/base/strings/strings.h"

namespace reactor {
namespace net {
namespace http {
namespace mimetypes {

using namespace std;
using namespace reactor::base;

typedef map<string, string> SuffixMap;
typedef map<string, string> EncodingMap;
typedef map<string, string> TypeMap;

static SuffixMap suffix_map = {
	{".svgz", ".svg.gz"},
	{".tgz", ".tar.gz"},
	{".taz", ".tar.gz"},
	{".tz", ".tar.gz"},
	{".tbz2", ".tar.bz2"},
	{".txz", ".tar.xz"},
};

static EncodingMap encodings_map = {
	{".gz", "gzip"},
	{".Z", "compress"},
	{".bz2", "bzip2"},
	{".xz", "xz"},
};

// Before adding new types, make sure they are either registered with IANA,
// at http,//www.isi.edu/in-notes/iana/assignments/media-types
// or extensions, i.e. using the x- prefix
// If you add to these, please keep them sorted!
static TypeMap types_map = {
    {".a"      , "application/octet-stream"},
    {".ai"     , "application/postscript"},
    {".aif"    , "audio/x-aiff"},
    {".aifc"   , "audio/x-aiff"},
    {".aiff"   , "audio/x-aiff"},
    {".au"     , "audio/basic"},
    {".avi"    , "video/x-msvideo"},
    {".bat"    , "text/plain"},
    {".bcpio"  , "application/x-bcpio"},
    {".bin"    , "application/octet-stream"},
    {".bmp"    , "image/x-ms-bmp"},
    {".c"      , "text/plain"},
    // Duplicates ,(
    {".cdf"    , "application/x-cdf"},
    {".cdf"    , "application/x-netcdf"},
    {".cpio"   , "application/x-cpio"},
    {".csh"    , "application/x-csh"},
    {".css"    , "text/css"},
    {".csv"    , "text/csv"},
    {".dll"    , "application/octet-stream"},
    {".doc"    , "application/msword"},
    {".dot"    , "application/msword"},
    {".dvi"    , "application/x-dvi"},
    {".eml"    , "message/rfc822"},
    {".eps"    , "application/postscript"},
    {".etx"    , "text/x-setext"},
    {".exe"    , "application/octet-stream"},
    {".gif"    , "image/gif"},
    {".gtar"   , "application/x-gtar"},
    {".h"      , "text/plain"},
    {".hdf"    , "application/x-hdf"},
    {".htm"    , "text/html"},
    {".html"   , "text/html"},
    {".ico"    , "image/vnd.microsoft.icon"},
    {".ief"    , "image/ief"},
    {".jpe"    , "image/jpeg"},
    {".jpeg"   , "image/jpeg"},
    {".jpg"    , "image/jpeg"},
    {".js"     , "application/javascript"},
    {".ksh"    , "text/plain"},
    {".latex"  , "application/x-latex"},
    {".m1v"    , "video/mpeg"},
    {".man"    , "application/x-troff-man"},
    {".me"     , "application/x-troff-me"},
    {".mht"    , "message/rfc822"},
    {".mhtml"  , "message/rfc822"},
    {".mif"    , "application/x-mif"},
    {".mov"    , "video/quicktime"},
    {".movie"  , "video/x-sgi-movie"},
    {".mp2"    , "audio/mpeg"},
    {".mp3"    , "audio/mpeg"},
    {".mp4"    , "video/mp4"},
    {".mpa"    , "video/mpeg"},
    {".mpe"    , "video/mpeg"},
    {".mpeg"   , "video/mpeg"},
    {".mpg"    , "video/mpeg"},
    {".ms"     , "application/x-troff-ms"},
    {".nc"     , "application/x-netcdf"},
    {".nws"    , "message/rfc822"},
    {".o"      , "application/octet-stream"},
    {".obj"    , "application/octet-stream"},
    {".oda"    , "application/oda"},
    {".p12"    , "application/x-pkcs12"},
    {".p7c"    , "application/pkcs7-mime"},
    {".pbm"    , "image/x-portable-bitmap"},
    {".pdf"    , "application/pdf"},
    {".pfx"    , "application/x-pkcs12"},
    {".pgm"    , "image/x-portable-graymap"},
    {".pl"     , "text/plain"},
    {".png"    , "image/png"},
    {".pnm"    , "image/x-portable-anymap"},
    {".pot"    , "application/vnd.ms-powerpoint"},
    {".ppa"    , "application/vnd.ms-powerpoint"},
    {".ppm"    , "image/x-portable-pixmap"},
    {".pps"    , "application/vnd.ms-powerpoint"},
    {".ppt"    , "application/vnd.ms-powerpoint"},
    {".ps"     , "application/postscript"},
    {".pwz"    , "application/vnd.ms-powerpoint"},
    {".py"     , "text/x-python"},
    {".pyc"    , "application/x-python-code"},
    {".pyo"    , "application/x-python-code"},
    {".qt"     , "video/quicktime"},
    {".ra"     , "audio/x-pn-realaudio"},
    {".ram"    , "application/x-pn-realaudio"},
    {".ras"    , "image/x-cmu-raster"},
    {".rdf"    , "application/xml"},
    {".rgb"    , "image/x-rgb"},
    {".roff"   , "application/x-troff"},
    {".rtx"    , "text/richtext"},
    {".sgm"    , "text/x-sgml"},
    {".sgml"   , "text/x-sgml"},
    {".sh"     , "application/x-sh"},
    {".shar"   , "application/x-shar"},
    {".snd"    , "audio/basic"},
    {".so"     , "application/octet-stream"},
    {".src"    , "application/x-wais-source"},
    {".sv4cpio", "application/x-sv4cpio"},
    {".sv4crc" , "application/x-sv4crc"},
    {".svg"    , "image/svg+xml"},
    {".swf"    , "application/x-shockwave-flash"},
    {".t"      , "application/x-troff"},
    {".tar"    , "application/x-tar"},
    {".tcl"    , "application/x-tcl"},
    {".tex"    , "application/x-tex"},
    {".texi"   , "application/x-texinfo"},
    {".texinfo", "application/x-texinfo"},
    {".tif"    , "image/tiff"},
    {".tiff"   , "image/tiff"},
    {".tr"     , "application/x-troff"},
    {".tsv"    , "text/tab-separated-values"},
    {".txt"    , "text/plain"},
    {".ustar"  , "application/x-ustar"},
    {".vcf"    , "text/x-vcard"},
    {".wav"    , "audio/x-wav"},
    {".webm"   , "video/webm"},
    {".wiz"    , "application/msword"},
    {".wsdl"   , "application/xml"},
    {".xbm"    , "image/x-xbitmap"},
    {".xlb"    , "application/vnd.ms-excel"},
    // Duplicates ,(
    {".xls"    , "application/excel"},
    {".xls"    , "application/vnd.ms-excel"},
    {".xml"    , "text/xml"},
    {".xpdl"   , "application/xml"},
    {".xpm"    , "image/x-xpixmap"},
    {".xsl"    , "application/xml"},
    {".xwd"    , "image/x-xwindowdump"},
    {".zip"    , "application/zip"},
};
    
// These are non-standard types, commonly found in the wild.  They will
// only match if strict=0 flag is given to the API methods.
// Please sort these too
static TypeMap common_types = {
    {".jpg" , "image/jpg"},
    {".mid" , "audio/midi"},
    {".midi", "audio/midi"},
    {".pct" , "image/pict"},
    {".pic" , "image/pict"},
    {".pict", "image/pict"},
    {".rtf" , "application/rtf"},
    {".xul" , "text/xul"},
};

static void readfp(FILE *f) {
	char *line = 0;
	size_t n = 0;
	for (;;) {
		ssize_t nread = ::getline(&line, &n, f);
		vector<string> words = strings::split(line);
		if (nread <= 0)
			break;
		for (size_t i = 1; i < words.size(); i++) {
			if (words[i][0] == '#') 
				continue;
			add_type(words[0], words[i]);
		}
	}
	::free(line);
}


/////////////////////////////////////////////
//
// APIs
//
/////////////////////////////////////////////

void add_type(const std::string &type, const std::string &suffix) {
	types_map[suffix] = type;
	// TODO types_map_inv
}

void guess_type(const std::string &path, std::string *type, std::string *encoding) {
	pair<string, string> words = path::splitext(path::basename(path)); // (base, ext)
	while (suffix_map.find(words.second) != suffix_map.end()) {
		words = path::splitext(words.first + suffix_map[words.second]);
	}

	string base = words.first;
	string ext = words.second;
	LOG(Debug) << "base: " << base << " ext: " << ext;
	if (encodings_map.find(ext) != encodings_map.end() && encoding != 0) {
		*encoding = encodings_map[ext];
		LOG(Debug) << "encoding: " << *encoding;
	} 

	if (types_map.find(ext) != types_map.end() && type != 0) {
		*type = types_map[ext];
		LOG(Debug) << "type: " << *type;
		return;
	}
	
	ext = strings::lower(ext);
	if (types_map.find(ext) != types_map.end() && type != 0) {
		*type = types_map[ext];
		LOG(Debug) << "type: " << *type;
		return;
	}
}

void read_mime_types(const std::string &file, std::map<std::string, std::string> *mimetypes) {
	FILE *f = fopen(file.c_str(), "r");
	if (f) {
		readfp(f);
		if (mimetypes) {
			*mimetypes = types_map;
		}
		fclose(f);
	} else {
		LOG(Error) << strerror(errno);
	}
}

} // namespace mimetypes
} // namespace http
} // namespace net
} // namespace reactor
