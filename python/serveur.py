
#include <sys/types.h>
#include <sys/sochet.h>
#!/usr/bin/env python

import socket, sys, time, os
import threading
import urllib
from subprocess import Popen, PIPE


        
class ThreadClient(threading.Thread):
        #derivation d'un objet thread pour geerer la connexion avec un client
        def __init__(self, conn):
                threading.Thread.__init__(self)
                self.connexion = conn

        #def run(self):
        #        # Dialogue avec le client :
        #        nom = self.getName()    # Chaque thread possede un nom
        #        while 1:
        #                msgClient = self.connexion.recv(1024)
        #                if msgClient.upper() == "FIN" or msgClient =="":
        #                        break
        #                message = "%s> %s" % (nom, msgClient)
        #                print message
        #                # Faire suivre le message a tous les autres clients :
        #                for cle in conn_client:
        #                        if cle != nom:	# ne pas le renvoyer a l'emetteur
        #                                conn_client[cle].send(message)
        #                # Fermeture de la connexion
        #                self.connexion.close()  # couper la connexion cote serveur
        #                del conn_client[nom]   # supprimer son entree dans le dictionnaire
        #                print "Client %s deconnecte." % nom
        #                # Le thread se termine ici
        
        
HOST = ''
PORT = 7000

# Initialisation du serveur - Mise en place du socket :
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

try :
        s.bind((HOST, PORT))
except socket.error :
        print (time.asctime()," : La liaison du socket a l'adresse choisie a echoue.")
        sys.exit()
print (time.asctime()," : Serveur pret, en attente de requetes ...")
s.listen(5)

# Attente et prise en charge des connexions demandees par les clients
conn_client = {}        # dictionnaire des connexions clients
while 1 :
        # On affiche l'adresse du client
        connexion, adresse = s.accept()
        # Creer un nouvel objet thread pour gerer la connexion
        th = ThreadClient(connexion)
        th.start()
        # Memoriser la connexion dans le dictionnaire
        it = th.getName()       # identifiant du thread
        conn_client[it] = connexion
        print "Client %s connecte, adresse IP %s, port %s." %\
                (it, adresse[0], adresse[1]) 
   
  
   
        # On ouvre le fichier log.txt et on ecrit les adresses dedans 
        log = open("log.txt", 'a')
        ligne = time.asctime() +" : Connected by " + str(adresse)
        if ligne == '':
                break
        else:
                log.write(ligne + '\n')
        log.close()
   
   
        data = connexion.recv(1024).decode()

        if not data :
                break
        #else :
        #        
        #        connexion.send(data.encode())
        
        print(str(data))
                
        if 'POST' in str(data) :
                chaine = str(data)
                ip = chaine.split("ip=")[1].split("&")[0]
                commande = chaine.split("commande=")[1].split("&")[0]
                
                
                # la commande n'est pas la meme en fonction du systeme d'exploitation
                #type = sys.platform
                #print(type)
                #
                #if type == "darwin" :
                #        if commande == "tracert":
                #                commande = "traceroute"
                #        elif commande == "ping":
                #                commande = "ping -c 3"
                #
                
                
                home = open("Home.html", 'a')
                p = Popen([commande, ip], stdout=PIPE)
                while True:
                        line = p.stdout.readline()
                        connexion.send(line)
                        print(line)
                        if not line:
                                break
                home.close()
                
        
        
        if 'GET' in str(data) :
                
                if 'favicon.ico' in str(data):
                        
                        favicon = open("favicon.ico", 'rb')
                        connexion.send(favicon.read())
                        favicon.close()
                        
                                
                elif 'Home.html' in str(data):
                        home = open("Home.html", 'r')
                        while 1:
                                ligne = home.readline()
                                if ligne == "":
                                        break
       
                                data = ligne
                                connexion.send(data.encode())
                        home.close()
                
                elif 'index.html' in str(data):
                        # On envoi ligne par ligne le fichier index.html
                        of = open("index.html", 'r')
                        while 1:
                                ligne = of.readline()
                                if ligne == "":
                                        break
       
                                data = ligne
                                connexion.send(data.encode())
                        of.close()
                        
                       
                        
                else :
                        # On envoi ligne par ligne le fichier erreur.html
                        erreur = open("erreur.html", 'r')
                        while 1:
                                ligne = erreur.readline()
                                if ligne == "":
                                        break
       
                                data = ligne
                                connexion.send(data)
                        erreur.close()
                
         
        
   
   
        connexion.close()
