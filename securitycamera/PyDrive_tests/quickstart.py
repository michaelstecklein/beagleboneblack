from pydrive.auth import GoogleAuth
from pydrive.drive import GoogleDrive




credentialsFile = "credentials.txt"

print "Getting authorization..."
gauth = GoogleAuth()
# Try to load saved client credentials
gauth.LoadCredentialsFile(credentialsFile)

if gauth.credentials is None:
    # Authenticate if they're not there
    gauth.LocalWebserverAuth()
    #gauth.CommandLineAuth()
elif gauth.access_token_expired:
    # Refresh them if expired
    gauth.Refresh()
else:
    # Initialize the saved creds
    gauth.Authorize()
# Save the current credentials to a file
gauth.SaveCredentialsFile(credentialsFile)

drive = GoogleDrive(gauth)

print "Creating file..."
textfile = drive.CreateFile({'title': 'Hello.txt'})  # Create GoogleDriveFile instance with title 'Hello.txt'.
print "Setting contents..."
textfile.SetContentString('Hello World!') # Set content of the file from given string.
print "Uploading..."
textfile.Upload()
print "Done"
