import subprocess
import os
import sys

current_dir = os.getcwd()
current_repo = os.path.basename(current_dir)

print(current_dir, current_repo)

# process = subprocess.Popen(["git", "fetch", "-a"], stdout=subprocess.PIPE)
# process = subprocess.Popen(["git", "remote"], stdout=subprocess.PIPE)
# process.wait()
# remotes = process.stdout.read().decode().split('\n')

remotes = "ALIDOR LILOU:https://gitlab.com/MissBidule/imac_algoprog_2.git," \
          "AMRANI BENJAMIN:https://gitlab.com/Banjomaestro/td-imac.git," \
          "APALOO KEZIAH:https://gitlab.com/KeziahAK/imac_algoprog_2.git," \
          "AUGEY QUENTIN:https://gitlab.com/quentin.augey/imac_algoprog_2," \
          "AUGIER LUCIE:https://gitlab.com/Lu6A/imac_algoprog_2," \
          "BEKKAR FADY:https://gitlab.com/fadybekkar77/projet_tps.git," \
          "BOULANGER ELISA:https://gitlab.com/Rousssse/imacalgosem-2-boulanger.git," \
          "CHAPELLE ROSE:https://github.com/Rosiflette/IMAC_S2_Prog," \
          "COUARD LOU:https://gitlab.com/Couloir00/imac_algoprog_2," \
          "COUARD THÉO:https://gitlab.com/TheoCrd/imac_algoprog_2.git," \
          "CREPIN OLIVIA:https://gitlab.com/olivia-ddow/imac_algoprog_2," \
          "DEBEAUNE TRISTAN:https://gitlab.com/debeaunetristan/imac_algoprog_2.git," \
          "DELCAMBRE ADRIEN:https://gitlab.com/adrrr2000/imac_algoprog_2.git," \
          "DIEP AUDREY:https://github.com/diepaudrey/S2_IMAC-Prog," \
          "DONA AXEL:https://github.com/AxelDona/IMAC_AlgoProg_2," \
          "DUGU LISE:https://gitlab.com/EliseDugue/imac_algoprog_2," \
          "ENRICO FLAVIE:https://gitlab.com/FlavieEnrico/imac_algoprog_2.git," \
          "FLEITZ MAELLE:https://gitlab.com/matraellix/imac_algoprog_2," \
          "GELEBART LAURIANE:https://gitlab.com/LaurianeGelebart/imac_algoprog_2.git," \
          "GERVAIS WENDY:https://gitlab.com/rubykiara1712/imac_algoprog_2.git," \
          "GOB PAULINE:https://gitlab.com/poupsipoups/IMAC-ALGO2," \
          "GODARD LUCIE:https://gitlab.com/lucie1704/IMAC_Prog2," \
          "GRIGNOLA NINA:https://gitlab.com/grignolanina/imac_algoprog_2.git," \
          "HUET QUENTIN:https://gitlab.com/quent1core/imac_algoprog_2," \
          "JEANNIN JULIETTE:https://gitlab.com/Juliette_J/imac_algoprog_2.git," \
          "LAFAURIE AURORE:https://github.com/Just-Kiel/Algo-Prog-S2," \
          "LE PAPE CHARLINE:https://gitlab.com/charlinelp/imac_algoprog_2.git," \
          "LEBLOND ANTOINE:https://gitlab.com/SlayerOfShadow/imac_algoprog_2," \
          "LECLERCQ MATT:https://gitlab.com/MatteoL-W/imac_algoprog_2," \
          "MASSA ELISE:https://gitlab.com/elise.massa.perso/imac_algoprog_2," \
          "MATHEVET MAXIME:https://gitlab.com/maXlam3nac01/imac_algoprog_2.git," \
          "N'GOTTA SARAH:https://gitlab.com/amlanstudio/imac_algoprog_2," \
          "POIROUX REMI:," \
          "PORA AUDE:https://gitlab.com/odewitness/imac_algoprog_2," \
          "QUINQUENEL LOIC:https://gitlab.com/loicqql/imac_algoprog_2," \
          "RAMBAUD MATHURIN:https://gitlab.com/rambaudmathurin/imac_algoprog_2," \
          "RAZAFI BENJAMIN:https://gitlab.com/benjoraz/imac_algoprog_2," \
          "ROARD ELIA:https://github.com/nwa7/TPalgos2," \
          "ROSALES PHILIPPE:https://gitlab.com/Audiophil15/imac_algoprog_2.git," \
          "SERRES ROMAIN:https://gitlab.com/Bouillon2Poulet/imac_algoprog_2.git," \
          "STRICH EMILY-ROSE:https://gitlab.com/emilyrosest/imac_algoprog_2," \
          "THIBERGE ERIC:https://gitlab.com/DANLCARTON/imac_algoprog_2," \
          "TINNES GREGOIRE:https://gitlab.com/Greg0s/imac_algoprog_2," \
          "VEAUVY EMMA:," \
          "WANG LAURENCE:https://gitlab.com/wanglaurence13/imac_algoprog_2"

if __name__ == '__main__':

    for remote in remotes.split(','):
        remote = remote.strip()
        remote, remote_url = remote.split(':', 1)
        if remote:
            if not remote_url:
                print("WARNING: Cannot find {}'s repo".format(remote))
                continue
            # process = subprocess.Popen(["git", "remote", "get-url", remote], stdout=subprocess.PIPE)
            # process.wait()
            # remote_url = process.stdout.read().decode().split('\n')[0]
            # print(remote, remote_url)

            user_dir = os.path.join(current_dir, '..', remote.replace(' ', '_'))
            if not os.path.isdir(user_dir) or not os.listdir(user_dir):
                try:
                    subprocess.check_call(["git", "clone", remote_url, user_dir], cwd=current_dir)
                except subprocess.SubprocessError as e:
                    print(e)

            try:
                os.remove(os.path.join(user_dir, ".git/rebase-merge"))
            except Exception as e:
                print(e)

            if not os.path.isdir(user_dir):
                print("WARNING: {}'s repo ({}) cannot be fetched !".format(remote, remote_url))
                continue

            subprocess.Popen(["git", "fetch", "-a"], cwd=user_dir).wait()
            subprocess.Popen(["git", "checkout", "--theirs", "."], cwd=user_dir).wait()
            try:
                subprocess.check_call(["git", "pull", "-X", "theirs", "origin", "master"], cwd=user_dir)
            except:
                subprocess.Popen(["git", "pull", "-X", "theirs", "origin", "main"], cwd=user_dir).wait()
