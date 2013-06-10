#include <CRegExp.h>

//#define BOOST_REGEX
#define TRE_REGEX

#if defined(BOOST_REGEX)
#include <boost/regex.h>
#elif defined(TRE_REGEX)
#include <tre/regex.h>
#else
#include <regexp/regexp.h>
#endif

#include <CStrUtil.h>

class CRegExpImpl {
 public:
  CRegExpImpl(const std::string &pattern="");
  CRegExpImpl(const CRegExpImpl &regexp);
 ~CRegExpImpl();

  const CRegExpImpl &operator=(const CRegExpImpl &regexp);

  void setPattern(const std::string &pattern);

  void setCaseSensitive(bool flag);
  void setExtended(bool flag);
  void setMatchBOL(bool flag);
  void setMatchEOL(bool flag);

  const std::string &getPattern() const { return pattern_; }

  bool find(const std::string &str) const;

  std::string replace(const std::string &str, bool global=false) const;

  bool isMatchAll() const;

  bool getMatchRange(int *start, int *end) const;

  bool getMatchRange(int pos, int *start, int *end) const;

  void getMatches(std::vector<CRegExpMatch> &matches) const;

  int         getNumMatches() const;
  std::string getMatchString(int i) const;

  bool isValid() const;

  std::string getErrorMsg() const;

 private:
  void compile();

  void setError(int error);

 private:
#if defined(BOOST_REGEX)
  boost::regex_tA     regex_;
  regmatch_t         *regmatch_;
#elif defined(TRE_REGEX)
  regex_t             regex_;
  regmatch_t         *regmatch_;
#else
  regexp::regex_t     regex_;
  regexp::regmatch_t *regmatch_;
#endif
  int                 num_regmatch_;
  std::string         pattern_;
  std::string         str_;
  bool                case_sensitive_;
  bool                extended_;
  bool                match_bol_;
  bool                match_eol_;
  bool                compiled_;
  bool                is_regex_;
  std::string         error_msg_;
};

CRegExp::
CRegExp(const std::string &pattern)
{
  impl_ = new CRegExpImpl(pattern);
}

CRegExp::
CRegExp(const CRegExp &regexp)
{
  impl_ = new CRegExpImpl(*regexp.impl_);
}

const CRegExp &
CRegExp::
operator=(const CRegExp &regexp)
{
  impl_ = new CRegExpImpl(*regexp.impl_);

  return *this;
}

CRegExp::
~CRegExp()
{
  delete impl_;
}

void
CRegExp::
setPattern(const std::string &pattern)
{
  impl_->setPattern(pattern);
}

void
CRegExp::
setCaseSensitive(bool flag)
{
  impl_->setCaseSensitive(flag);
}

void
CRegExp::
setExtended(bool flag)
{
  impl_->setExtended(flag);
}

void
CRegExp::
setMatchBOL(bool flag)
{
  impl_->setMatchBOL(flag);
}

void
CRegExp::
setMatchEOL(bool flag)
{
  impl_->setMatchEOL(flag);
}

const std::string &
CRegExp::
getPattern() const
{
  return impl_->getPattern();
}

bool
CRegExp::
find(const std::string &str) const
{
  return impl_->find(str);
}

std::string
CRegExp::
replace(const std::string &str, bool global) const
{
  return impl_->replace(str, global);
}

bool
CRegExp::
isMatchAll() const
{
  return impl_->isMatchAll();
}

bool
CRegExp::
getMatchRange(int *start, int *end) const
{
  return impl_->getMatchRange(start, end);
}

bool
CRegExp::
getMatchRange(int pos, int *start, int *end) const
{
  return impl_->getMatchRange(pos, start, end);
}

void
CRegExp::
getMatches(std::vector<CRegExpMatch> &matches) const
{
  return impl_->getMatches(matches);
}

void
CRegExp::
getMatches(std::vector<std::string> &matches) const
{
  int num = getNumMatches();

  matches.clear();

  for (int i = 0; i < num; ++i)
    matches.push_back(getMatchString(i));
}

int
CRegExp::
getNumMatches() const
{
  return impl_->getNumMatches();
}

std::string
CRegExp::
getMatchString(int pos) const
{
  return impl_->getMatchString(pos);
}

bool
CRegExp::
isValid() const
{
  return impl_->isValid();
}

std::string
CRegExp::
getErrorMsg() const
{
  return impl_->getErrorMsg();
}

bool
CRegExp::
find(const std::string &pattern, const std::string &str)
{
  CRegExpImpl regexp(pattern);

  return regexp.find(str);
}

std::string
CRegExp::
replace(const std::string &pattern, const std::string &str, bool global)
{
  CRegExpImpl regexp(pattern);

  return regexp.replace(str, global);
}

//---------------------------

CRegExpImpl::
CRegExpImpl(const std::string &pattern) :
 pattern_       (pattern),
 case_sensitive_(true),
 extended_      (false),
 match_bol_     (true),
 match_eol_     (true),
 compiled_      (false),
 is_regex_      (false)
{
#if defined(BOOST_REGEX)
  regmatch_     = NULL;
  num_regmatch_ = 0;
#elif defined(TRE_REGEX)
  regmatch_     = NULL;
  num_regmatch_ = 0;
#else
  regmatch_     = NULL;
  num_regmatch_ = 0;
#endif
}

CRegExpImpl::
CRegExpImpl(const CRegExpImpl &regexp) :
 pattern_       (regexp.pattern_),
 case_sensitive_(regexp.case_sensitive_),
 extended_      (regexp.extended_),
 match_bol_     (regexp.match_bol_),
 match_eol_     (regexp.match_eol_),
 compiled_      (false),
 is_regex_      (regexp.is_regex_)
{
#if defined(BOOST_REGEX)
  regmatch_     = NULL;
  num_regmatch_ = 0;
#elif defined(TRE_REGEX)
  regmatch_     = NULL;
  num_regmatch_ = 0;
#else
  regmatch_     = NULL;
  num_regmatch_ = 0;
#endif
}

const CRegExpImpl &
CRegExpImpl::
operator=(const CRegExpImpl &regexp)
{
  pattern_        = regexp.pattern_;
  case_sensitive_ = regexp.case_sensitive_;
  extended_       = regexp.extended_;
  match_bol_      = regexp.match_bol_;
  match_eol_      = regexp.match_eol_;
  compiled_       = false;
  is_regex_       = regexp.is_regex_;

#if defined(BOOST_REGEX)
  regmatch_     = NULL;
  num_regmatch_ = 0;
#elif defined(TRE_REGEX)
  regmatch_     = NULL;
  num_regmatch_ = 0;
#else
  regmatch_     = NULL;
  num_regmatch_ = 0;
#endif

  return *this;
}

CRegExpImpl::
~CRegExpImpl()
{
#if defined(BOOST_REGEX)
  delete [] regmatch_;
#elif defined(TRE_REGEX)
  delete [] regmatch_;
#else
  delete [] regmatch_;
#endif
}

void
CRegExpImpl::
setPattern(const std::string &pattern)
{
  pattern_  = pattern;
  compiled_ = false;
}

void
CRegExpImpl::
setCaseSensitive(bool flag)
{
  case_sensitive_ = flag;
}

void
CRegExpImpl::
setExtended(bool flag)
{
  extended_ = flag;
}

void
CRegExpImpl::
setMatchBOL(bool flag)
{
  match_eol_ = flag;
}

void
CRegExpImpl::
setMatchEOL(bool flag)
{
  match_bol_ = flag;
}

void
CRegExpImpl::
compile()
{
#if defined(BOOST_REGEX)
  int flags = 0;

  setError(0);

  if (! case_sensitive_)
    flags |= REG_ICASE;

  if (extended_)
    flags |= REG_EXTENDED;

  int error = regcompA(&regex_, pattern_.c_str(), flags);

  if (error != 0) {
    setError(error);

    is_regex_ = false;
  }
  else
    is_regex_ = true;
#elif defined(TRE_REGEX)
  int flags = 0;

  setError(0);

  if (! case_sensitive_)
    flags |= REG_ICASE;

  if (extended_)
    flags |= REG_EXTENDED;

  int error = regcomp(&regex_, pattern_.c_str(), flags);

  if (error != 0) {
    setError(error);

    is_regex_ = false;
  }
  else
    is_regex_ = true;
#else
  int flags = 0;

  setError(0);

  if (! case_sensitive_)
    flags |= REG_ICASE;

  if (extended_)
    flags |= REG_EXTENDED;

  int error = regexp::regcomp(&regex_, pattern_.c_str(), flags);

  if (error != 0) {
    setError(error);

    is_regex_ = false;
  }
  else
    is_regex_ = true;
#endif

  compiled_ = true;
}

bool
CRegExpImpl::
find(const std::string &str) const
{
  CRegExpImpl *th = const_cast<CRegExpImpl *>(this);

  if (! compiled_)
    th->compile();

  th->str_ = str;

  th->setError(0);

  int rc;

  if (is_regex_) {
#if defined(BOOST_REGEX)
    if (regmatch_ == NULL) {
      th->num_regmatch_ = 100;

      th->regmatch_ = new regmatch_t [num_regmatch_];
    }

    int flags = 0;

    if (! match_bol_) flags |= REG_NOTBOL;
    if (! match_eol_) flags |= REG_NOTEOL;

    int error = regexecA(&regex_, str_.c_str(), num_regmatch_,
                         regmatch_, flags);

    if (error != 0) {
      th->setError(error);

      rc = 1;
    }
    else
      rc = 0;
#elif defined(TRE_REGEX)
    if (regmatch_ == NULL) {
      th->num_regmatch_ = 100;

      th->regmatch_ = new regmatch_t [num_regmatch_];
    }

    int flags = 0;

    if (! match_bol_) flags |= REG_NOTBOL;
    if (! match_eol_) flags |= REG_NOTEOL;

    int error = regexec(&regex_, str_.c_str(), num_regmatch_,
                        regmatch_, flags);

    if (error != 0) {
      th->setError(error);

      rc = 1;
    }
    else
      rc = 0;
#else
    if (regmatch_ == NULL) {
      th->num_regmatch_ = 100;

      th->regmatch_ = new regexp::regmatch_t [num_regmatch_];
    }

    int flags = 0;

    if (! match_bol_) flags |= REG_NOTBOL;
    if (! match_eol_) flags |= REG_NOTEOL;

    int error = regexp::regexec(&regex_, str_.c_str(), num_regmatch_,
                                regmatch_, flags);

    if (error != 0) {
      th->setError(error);

      rc = 1;
    }
    else
      rc = 0;
#endif
  }
  else {
    if (case_sensitive_)
      rc = pattern_.compare(str_);
    else
      rc = CStrUtil::casecmp(pattern_, str_);
  }

  return (rc == 0);
}

std::string
CRegExpImpl::
replace(const std::string &str, bool global) const
{
#if 0
  {
  cerr << "Range(0):" << regmatch_[0].rm_so << ":" <<
          regmatch_[0].rm_eo << endl;

  int pos = 1;

  for (int i = 1; i < num_regmatch_; ++i) {
    if (regmatch_[i].rm_so == -1)
      continue;

    cerr << "Range(" << pos << "):" << regmatch_[pos].rm_so << ":" <<
            regmatch_[pos].rm_eo << endl;

    ++pos;
  }
  }
#endif

  std::string lhs = str_.substr(0, regmatch_[0].rm_so);
  std::string rhs = str_.substr(regmatch_[0].rm_eo);

  uint len = str.size();

  std::string res = lhs;

  for (uint pos = 0; pos < len; ++pos) {
    if (str[pos] == '\\' && pos < len - 1) {
      ++pos;

      if (isdigit(str[pos])) {
        int num = str[pos] - '0';

        while (pos < len - 1 && isdigit(str[pos + 1])) {
          ++pos;

          num = num*10 + str[pos] - '0';
        }

        res += getMatchString(num - 1);
      }
      else
        res += str[pos];
    }
    else if (str[pos] == '&')
      res += str_;
    else
      res += str[pos];
  }

  if (global) {
    if (find(rhs))
      res += replace(str, global);
    else
      res += rhs;
  }
  else
    res += rhs;

  return res;
}

bool
CRegExpImpl::
isMatchAll() const
{
  int start, end;

  if (! getMatchRange(&start, &end))
    return false;

  return (start == 0 && end == (int) str_.size() - 1);
}

bool
CRegExpImpl::
getMatchRange(int *start, int *end) const
{
  if (num_regmatch_ == 0 || regmatch_[0].rm_so == -1)
    return false;

  *start = regmatch_[0].rm_so;
  *end   = regmatch_[0].rm_eo - 1;

  return true;
}

bool
CRegExpImpl::
getMatchRange(int pos, int *start, int *end) const
{
  int pos1 = 1;

  for (int i = 1; i < num_regmatch_; ++i) {
    if (regmatch_[i].rm_so == -1)
      continue;

    if (pos == pos1) {
      *start = regmatch_[pos1].rm_so;
      *end   = regmatch_[pos1].rm_eo - 1;

      return true;
    }

    ++pos1;
  }

  return false;
}

void
CRegExpImpl::
getMatches(std::vector<CRegExpMatch> &matches) const
{
  for (int i = 1; i < num_regmatch_; ++i) {
    if (regmatch_[i].rm_so == -1)
      continue;

    matches.push_back(CRegExpMatch(regmatch_[i].rm_so, regmatch_[i].rm_eo));
  }
}

int
CRegExpImpl::
getNumMatches() const
{
  int num = 0;

  for (int i = 1; i < num_regmatch_; ++i) {
    if (regmatch_[i].rm_so == -1)
      break;

    ++num;
  }

  return num;
}

std::string
CRegExpImpl::
getMatchString(int pos) const
{
  int num = 0;

  for (int i = 1; i < num_regmatch_; ++i) {
    if (regmatch_[i].rm_so == -1)
      break;

    if (num == pos)
      return str_.substr(regmatch_[i].rm_so,
                         regmatch_[i].rm_eo - regmatch_[i].rm_so);

    ++num;
  }

  return "";
}

void
CRegExpImpl::
setError(int error)
{
  static char buffer[256];

  if (error != 0) {
#if defined(BOOST_REGEX)
    size_t size = regerrorA(error, &regex_, buffer, 256);
#elif defined(TRE_REGEX)
    size_t size = regerror(error, &regex_, buffer, 256);
#else
    size_t size = regexp::regerror(error, &regex_, buffer, 256);
#endif

    if (size < 256)
      buffer[size] = '\0';

    error_msg_ = buffer;
  }
  else
    error_msg_ = "";
}

bool
CRegExpImpl::
isValid() const
{
  CRegExpImpl *th = const_cast<CRegExpImpl *>(this);

  if (! compiled_)
    th->compile();

  return is_regex_;
}

std::string
CRegExpImpl::
getErrorMsg() const
{
  return error_msg_;
}

//----------

bool
CRegExpUtil::
parse(const std::string &str, const std::string &pattern)
{
  CRegExp regexp(pattern);

  return parse(str, regexp);
}

bool
CRegExpUtil::
parse(const std::string &str, const CRegExp &regexp)
{
  return regexp.find(str);
}

bool
CRegExpUtil::
parse(const std::string &str, const std::string &pattern, std::vector<std::string> &match_strs)
{
  CRegExp regexp(pattern);

  return parse(str, regexp, match_strs);
}

bool
CRegExpUtil::
parse(const std::string &str, const CRegExp &regexp, std::vector<std::string> &match_strs)
{
  if (! regexp.find(str))
    return false;

  regexp.getMatches(match_strs);

  return true;
}

int
CRegExpUtil::
gregsub(const std::string &istr, const CRegExp &pattern, const std::string &rstr, std::string &ostr)
{
  int count = 0;

  ostr = "";

  std::string istr1 = istr;

  while (pattern.find(istr1)) {
    ++count;

    int spos, epos;

    if (! pattern.getMatchRange(&spos, &epos))
      return false;

    if (spos > 0)
      ostr += istr1.substr(0, spos);

    std::string rstr1;

    uint rlen = rstr.size();

    for (uint i = 0; i < rlen; ++i) {
      if      (rstr[i] == '\\') {
        rstr1 += rstr[i++];

        if (i < rlen)
          rstr1 += rstr[i];
      }
      else if (rstr[i] == '&')
        rstr1 += istr1.substr(spos, epos - spos + 1);
      else
        rstr1 += rstr[i];
    }

    ostr += rstr1;

    istr1 = istr1.substr(epos + 1);
  }

  ostr += istr1;

  return count;
}

bool
CRegExpUtil::
regsub(const std::string &istr, const CRegExp &pattern, const std::string &rstr, std::string &ostr)
{
  if (! pattern.find(istr))
    return false;

  int spos, epos;

  if (! pattern.getMatchRange(&spos, &epos))
    return false;

  if (spos > 0)
    ostr += istr.substr(0, spos);

  std::string rstr1;

  uint rlen = rstr.size();

  for (uint i = 0; i < rlen; ++i) {
    if      (rstr[i] == '\\') {
      rstr1 += rstr[i++];

      if (i < rlen)
        rstr1 += rstr[i];
    }
    else if (rstr[i] == '&')
      rstr1 += istr.substr(spos, epos - spos + 1);
    else
      rstr1 += rstr[i];
  }

  ostr += rstr1;

  ostr += istr.substr(epos + 1);

  return true;
}
