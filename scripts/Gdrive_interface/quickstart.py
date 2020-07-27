from __future__ import print_function
import pickle
import os.path
from googleapiclient.discovery import build
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from apiclient.http import MediaFileUpload

# If modifying these scopes, delete the file token.pickle.
SCOPES = ['https://www.googleapis.com/auth/drive']

#driveId="0AMlf51fRAjtrUk9PVA" for UCSC UFSD Group



def main():
    """Shows basic usage of the Drive v3 API.
    Prints the names and ids of the first 10 files the user has access to.
    """
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

    service = build('drive', 'v3', credentials=creds)

    # Call the Drive v3 API
    results = service.files().list(
        pageSize=20, driveId="0AMlf51fRAjtrUk9PVA",
        includeItemsFromAllDrives=True, corpora="drive",
        supportsAllDrives=True,
        fields="nextPageToken, files(id, name)"
	).execute()

    items = results.get('files', [])
    #print (items)
    if False:
        if not items:
            print('No files found.')
        else:
            print('Files:')
            for item in items:
                print(u'{0} ({1})'.format(item['name'], item['id']))

    if True:
        file_metadata = {
            'name': 'Test',
            'description':'Test folder',
            'parents':['0AMlf51fRAjtrUk9PVA'],
            'mimeType': 'application/vnd.google-apps.folder'
        }

        file = service.files().create(  body=file_metadata,
                                        supportsAllDrives=True,
                                        fields='id').execute()
        print (f"Folder ID: {file.get('id')}")

    if True:
        file_metadata = {'name': 'Plot_Sensor_bias_voltage_vs_charge.pdf',
                        'parents': [file.get('id')]}
        media = MediaFileUpload('Plot_Sensor_bias_voltage_vs_charge.pdf',
                                mimetype='image/pdf', resumable=True)
        file = service.files().create(  body=file_metadata,
                                        media_body=media,
                                        supportsAllDrives=True,
                                        fields='id').execute()
        print (f"File ID: {file.get('id')}")


if __name__ == '__main__':
    main()
