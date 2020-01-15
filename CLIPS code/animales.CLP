(defrule mamifero
   (sangre 1)
   (piel 1)
   (mover 1)
=>
   (printout t "mamifero." crlf)
)

(defrule ave
   (sangre 1)
   (piel 2)
   (mover 2)
=>
   (printout t "ave." crlf)
)

(defrule reptil
   (sangre 2)
   (piel 3)
=>
   (printout t "reptil." crlf)
)

(defrule anfibio
   (sangre 2)
   (piel 4)
=>
   (printout t "anfibio." crlf)
)

(defrule input
   (initial-fact)
=>
   (printout t crlf "¿Como es su sangre? (1: caliente, 2: fria) ")
   (assert (sangre =(read)))

   (printout t crlf "¿Como es su piel? 
   (1: con pelo, 2: con plumas, 3: con escamas, 4: con poros) ")
   (assert (piel =(read)))

   (printout t crlf "¿Como se mueve?
   (1: andando, 2: volando) ")
   (assert (mover =(read)))
   (printout t crlf "El animal es un " crlf)
)