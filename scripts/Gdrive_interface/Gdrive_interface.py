from __future__ import print_function
import pickle
import os.path
from googleapiclient.discovery import build
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from apiclient.http import MediaFileUpload

# If modifying these scopes, delete the file token.pickle.
SCOPES = ['https://www.googleapis.com/auth/drive']

# driveId="0AMlf51fRAjtrUk9PVA" for UCSC UFSD Group
# Central data folder ID: 1zJNvXQV8YGAj5HdPVCFn0xBjYr9fZb9b
class gdrive_interface():
    def __init__(self):
        self.UCSC_shared_driveID = "0AMlf51fRAjtrUk9PVA"
        self.central_folderID = "1zJNvXQV8YGAj5HdPVCFn0xBjYr9fZb9b"
        creds = None
        # The file token.pickle stores the user's access and refresh tokens, and is
        # created automatically when the authorization flow completes for the first
        # time.
        if os.path.exists('token.pickle'):
            with open('token.pickle', 'rb') as token:
                creds = pickle.load(token)
        # If there are no (valid) credentials available, let the user log in.
        if not creds or not creds.valid:
            if creds and creds.expired and creds.refresh_token:
                creds.refresh(Request())
            else:
                flow = InstalledAppFlow.from_client_secrets_file(
                    'credentials.json', SCOPES)
                creds = flow.run_local_server(port=0)
            # Save the credentials for the next run
            with open('token.pickle', 'wb') as token:
                pickle.dump(creds, token)

        self.gdrive_service = build('drive', 'v3', credentials=creds)

    def create_folder(self, folder_name, folder_parents = []):
        #if not specified put in UCSC shared drive
        if len(folder_parents) == 0: folder_parents = [self.UCSC_shared_driveID]
        file_metadata = {
            'name': folder_name,
            'parents': folder_parents,
            'mimeType': 'application/vnd.google-apps.folder'
        }

        file = self.gdrive_service.files().create(  body=file_metadata,
                                                    supportsAllDrives=True,
                                                    fields='id').execute()
        return file.get('id')

    def upload_file(self, file_name, file_parents = []):
        #if not specified put in UCSC shared drive
        if len(file_parents) == 0: file_parents = [self.UCSC_shared_driveID]

        file_metadata = {'name': file_name,
                        'parents': [file_parents]}

        media = MediaFileUpload(file_name, resumable=True)

        file = self.gdrive_service.files().create(  body=file_metadata,
                                        media_body=media,
                                        supportsAllDrives=True,
                                        fields='id').execute()
        return file.get('id')

    def list_files(self, folder_id = ""):
        # standard is to return files in the shared drive
        if folder_id == "":
            results = self.gdrive_service.files().list( pageSize=20, driveId=self.UCSC_shared_driveID,
                                                        includeItemsFromAllDrives=True, corpora="drive",
                                                        supportsAllDrives=True,
                                                        fields="nextPageToken, files(id, name)"
                                                        ).execute()
            items = results.get('files', [])
        else:
            items = []
        return items


if __name__ == '__main__':
    gdrive = gdrive_interface()

    items = gdrive.list_files()
    for item in items:
        print(u'{0} ({1})'.format(item['name'], item['id']))

    #folderID = gdrive.create_folder("2020_Central_data_folder")
    #print (folderID)
