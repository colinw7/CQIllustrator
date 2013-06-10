#ifndef CQIllustratorCmd_H
#define CQIllustratorCmd_H

#include <CArgs.h>
#include <map>

class CQIllustrator;
class CQIllustratorCmd;

class CQIllustratorCmdMgr {
 public:
  CQIllustratorCmdMgr(CQIllustrator *illustrator);

  void addCommand(CQIllustratorCmd *cmd);

  bool execCmd(const std::string &text) const;

  void displayCmds() const;

 private:
  typedef std::map<std::string, CQIllustratorCmd *> CmdList;

  CQIllustrator *illustrator_;
  CmdList        cmdList_;
};

class CQIllustratorCmd {
 public:
  CQIllustratorCmd(const char *args);

  virtual ~CQIllustratorCmd() { }

  virtual const char *getName() const = 0;

  virtual bool exec(const std::vector<std::string> &words) = 0;

  void setIllustrator(CQIllustrator *illustrator) { illustrator_ = illustrator; }

 protected:
  CQIllustrator *illustrator_;
  CArgs          args_;
};

#endif
