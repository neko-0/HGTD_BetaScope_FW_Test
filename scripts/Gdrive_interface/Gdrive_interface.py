from __future__ import print_function
import pickle
import os.path
from googleapiclient.discovery import build
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from apiclient.http import MediaFileUpload, MediaIoBaseDownload
import io

# If modifying these scopes, delete the file token.pickle.
SCOPES = ['https://www.googleapis.com/auth/drive']

# Hardcoded and ugly
code_folder = "/home/datataking/HGTD_BetaScope_FW_Test/scripts/Gdrive_interface/"

#UDI_FILE = "1IgnXmQ9r2UbxzFZVLkMSLwS1D1F_BffOS8r9lcIvZG4"
# driveId="0AMlf51fRAjtrUk9PVA" for UCSC UFSD Group
# Central data folder ID: 1zJNvXQV8YGAj5HdPVCFn0xBjYr9fZb9b
class gdrive_interface():
    def __init__(self):
        self.UCSC_shared_driveID = "0AMlf51fRAjtrUk9PVA"
        self.central_folderID = "1zJNvXQV8YGAj5HdPVCFn0xBjYr9fZb9b"
        self.UDI_fileID = "1IgnXmQ9r2UbxzFZVLkMSLwS1D1F_BffOS8r9lcIvZG4"

        creds = None
        # The file token.pickle stores the user's access and refresh tokens, and is
        # created automatically when the authorization flow completes for the first
        # time.
        if os.path.exists(f'{code_folder}token.pickle'):
            with open(f'{code_folder}token.pickle', 'rb') as token:
                creds = pickle.load(token)

        # If there are no (valid) credentials available, let the user log in.
        if not creds or not creds.valid:
            if creds and creds.expired and creds.refresh_token:
                creds.refresh(Request())
            else:
                flow = InstalledAppFlow.from_client_secrets_file(
                    f'{code_folder}credentials.json', SCOPES)
                creds = flow.run_local_server(port=0)
            # Save the credentials for the next run
            with open(f'{code_folder}token.pickle', 'wb') as token:
                pickle.dump(creds, token)

        self.gdrive_service = build('drive', 'v3', credentials=creds)

    def create_folder(self, folder_name, folder_parents = []):
        #if not specified put in the central folder
        if len(folder_parents) == 0: folder_parents = [self.central_folderID]

        file_metadata = {
            'name': folder_name,
            'parents': folder_parents,
            'mimeType': 'application/vnd.google-apps.folder'
        }

        folder = self.gdrive_service.files().create(  body=file_metadata,
                                                    supportsAllDrives=True,
                                                    fields='id').execute()
        return folder

    def upload_file(self, file_name, file_parents = []):
        #if not specified put in the central folder
        if len(file_parents) == 0: file_parents = [self.central_folderID]

        file_metadata = {'name': file_name,
                        'parents': file_parents}

        media = MediaFileUpload(file_name, resumable=True)

        file = self.gdrive_service.files().create(  body=file_metadata,
                                                    media_body=media,
                                                    supportsAllDrives=True,
                                                    fields='id').execute()
        return file

    def download_UDI_file(self):
        file_id = self.UDI_fileID
        #file_id = "1QLGEk4yEoDULe1kic2LYLTdXR54iPKth"
        request = self.gdrive_service.files().export_media( fileId=file_id,
                                                            mimeType="application/vnd.openxmlformats-officedocument.spreadsheetml.sheet")
        fh = io.FileIO("UDI_list.xlsx", "wb")
        downloader = MediaIoBaseDownload(fh, request)
        done = False
        while done is False:
            status, done = downloader.next_chunk()
        print ("UDI list downloaded")

    def download_file(self, file_id):
        request = self.gdrive_service.files().get_media(fileId=file_id, supportsAllDrives=True)
        fh = io.FileIO("file", "wb")
        downloader = MediaIoBaseDownload(fh, request)
        done = False
        while done is False:
            status, done = downloader.next_chunk()
            print (f"Download {int(status.progress()*100)}")
        print ("File downloaded")

    def list_files(self, folder_id = ""):
        # standard is to return files in the shared drive central folder
        if folder_id == "": folder_id = self.central_folderID

        results = self.gdrive_service.files().list( pageSize=20, driveId=self.UCSC_shared_driveID,
                                                    q=f"'{folder_id}' in parents and trashed = false",
                                                    includeItemsFromAllDrives=True, corpora="drive",
                                                    supportsAllDrives=True,
                                                    fields="nextPageToken, files(id, name)"
                                                    ).execute()
        items = results.get('files', [])
        return items

    def empty_central_folder(self):
        folder_id = self.central_folderID
        items = self.list_files(folder_id)
        for item in items:
            print(u'Deleting: {0} ({1})'.format(item['name'], item['id']))
            self.delete_file(item['id'])

    def delete_file(self, file_id):
        # WARNING: DELETE not TRASH, file will be no longer available
        updated_file = self.gdrive_service.files().delete(  fileId=file_id,
                                                            supportsAllDrives=True,
                                                            ).execute()

if __name__ == '__main__':
    gdrive = gdrive_interface()
    #gdrive.empty_central_folder()
    items = gdrive.list_files()
    for item in items:
        print(u'{0} ({1})\n'.format(item['name'], item['id']))

    gdrive.download_UDI_file()
    #gdrive.download_file("1QLGEk4yEoDULe1kic2LYLTdXR54iPKth")
    #item = gdrive.upload_file("Test.pdf")
    #print(item['id'])

    #gdrive.delete_file("10BqYomDcmbiaGmbCD6b4cRCGNaHDN26S")
