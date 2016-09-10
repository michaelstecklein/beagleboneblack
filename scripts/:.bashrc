
# Vars
beagleboneblack="root@192.168.7.2"

# Setting PATH for Python 3.4
# The orginal version is saved in .bash_profile.pysave
PATH="/Library/Frameworks/Python.framework/Versions/3.4/bin:${PATH}"
export PATH

# Alias file
if [ -f ~/.bash_aliases ]; then
	. ~/.bash_aliases
fi
