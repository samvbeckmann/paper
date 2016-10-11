printf "Sam Beckmann 3561\nJournal %d\n\n" $1 > journal.log
git log --pretty=format:'%cd%n%s%n' --date=local --since=$2 >> journal.log
