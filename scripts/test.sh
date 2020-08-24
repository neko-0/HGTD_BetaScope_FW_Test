wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=1qCYdSienjzc9QlKpNPY3AJHNplcROWNr' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=1qCYdSienjzc9QlKpNPY3AJHNplcROWNr" -O test_run.zip && rm -rf /tmp/cookies.tx

unzip test_run.zip

./BetaScope_Ana/BetaScopeWaveformAna/bin/GenerateWaevformCofing test
./BetaScope_Ana/BetaScopeWaveformAna/bin/Run_WaveformAna -config WaveformAnaConfig.ini
