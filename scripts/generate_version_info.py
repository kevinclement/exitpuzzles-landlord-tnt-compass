import os,time
Import("env")

git_hash = os.popen('git --no-pager describe --tags --always --dirty').read().strip()
git_date = os.popen('git --no-pager show --date=format:"%Y-%m-%d %H:%M:%S" --format="%ad" --name-only').read().partition('\n')[0].strip()
date_now = time.strftime("%Y-%m-%d %H:%M:%S")

print("######### generate_git_header #############")
print("hash: '{0}'".format(git_hash))
print("date: '{0}'".format(git_date))
print("now:  '{0}'".format(date_now))
print("###########################################")

env.Append(CPPDEFINES=[
  ("GIT_HASH", '\\"' + git_hash + '\\"'),
  ("GIT_DATE", '\\"' + git_date + '\\"'),
  ("DATE_NOW", '\\"' + date_now + '\\"')
])