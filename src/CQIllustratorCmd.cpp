#include <CQIllustratorCmd.h>
#include <CQIllustrator.h>
#include <CStrUtil.h>

class CQIllustratorHelpCmd : public CQIllustratorCmd {
 public:
  CQIllustratorHelpCmd();

  const char *getName() const { return "help"; }

  bool exec(const std::vector<std::string> &words);
};

CQIllustratorCmdMgr::
CQIllustratorCmdMgr(CQIllustrator *illustrator) :
 illustrator_(illustrator)
{
  addCommand(new CQIllustratorHelpCmd);
}

void
CQIllustratorCmdMgr::
addCommand(CQIllustratorCmd *cmd)
{
  cmd->setIllustrator(illustrator_);

  std::string name = cmd->getName();

  cmdList_[CStrUtil::toLower(name)] = cmd;
}

bool
CQIllustratorCmdMgr::
execCmd(const std::string &text) const
{
  std::vector<std::string> words;

  CStrUtil::addWords(text, words);

  uint num_words = words.size();

  if (num_words == 0) return true;

  CmdList::const_iterator p = cmdList_.find(CStrUtil::toLower(words[0]));

  if (p == cmdList_.end())
    return false;

  return (*p).second->exec(words);
}

void
CQIllustratorCmdMgr::
displayCmds() const
{
  CmdList::const_iterator p1, p2;

  for (p1 = cmdList_.begin(), p2 = cmdList_.end(); p1 != p2; ++p1) {
    const CQIllustratorCmd *cmd = (*p1).second;

    std::cout << cmd->getName() << std::endl;
  }
}

//-------

CQIllustratorCmd::
CQIllustratorCmd(const char *args) :
 illustrator_(0), args_(args)
{
}

//-------

static const char *help_opts = "\
";

CQIllustratorHelpCmd::
CQIllustratorHelpCmd() :
 CQIllustratorCmd(help_opts)
{
}

bool
CQIllustratorHelpCmd::
exec(const std::vector<std::string> &words)
{
  args_.parse(words);

  CQIllustratorCmdMgr *cmdMgr = illustrator_->getCmdMgr();

  cmdMgr->displayCmds();

  return true;
}
