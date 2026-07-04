Option :

--help : show help menu

Bonus possible :

-m = ttl max (default = 30)
-f = Start from the N hop (instead from 1)
-q = nombre de probes (paquet) envoyé par TTL
-rdns = Résoudre l'ip addresse en nom de domaine
-z = temp entre deux probe (0 par défault)



Tests :

traceroute --> afiche le help menu
traceroute "" // OK
traceroute -m --> erreur manque le host // ok
traceroute --g // ok
traceroute --m // ok
traceroute ---m 8.8.8.8 -> ---m est considéré comme le host et affiche une erreur // ok
traceroute 0000a0 //ok
traceroute 60 8.8.8.8 //ok
traceroute -8.8.8.8 //ok

traceroute -4 -m -1 8.8.8.8 //ok
traceroute -4 -m 256 8.8.8.8 //ok
traceroute -4 -m -1 8.8.8.8 //ok
traceroute -4 -m 256 8.8.8.8 //ok

traceroute -f 256 8.8.8.8 //ok
traceroute -f 255 8.8.8.8 //ok
traceroute -f 300 8.8.8.8 //ok
traceroute -f -1 8.8.8.8 //ok

traceroute -q -1 8.8.8.8 //ok
traceroute -q 255 8.8.8.8 //ok

traceroute 8.8.8.8 -1 //ok
traceroute 8.8.8.8 60.1 // ok
traceroute -f 25 8.8.8.8 //ok
traceroute 8.8.8.8 65001

work :
traceroute 8.8.8.8
traceroute 8.8.8.8 60
traceroute -f 5 8.8.8.8
traceroute 8.8.8.8 65000




./ft_traceroute "$TARGET" > actual.txt 2>&1
traceroute "$TARGET" > expected.txt 2>&1

if diff -u expected.txt actual.txt; then
    echo "✅ Outputs identiques"
else
    echo "❌ Différences détectées"
fi