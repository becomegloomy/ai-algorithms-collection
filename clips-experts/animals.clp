; Вспомогательные функции

; Функция задающая пользователю вопрос и 
; получающая ответ, принадлежащий списку допустимых
(deffunction ask-question (?question $?allowed-values)
   (printout t ?question)
   (bind ?answer (read))
   (if (lexemep ?answer) 
       then (bind ?answer (lowcase ?answer)))
   (while (not (member ?answer ?allowed-values)) do
      (printout t ?question)
      (bind ?answer (read))
      (if (lexemep ?answer) 
          then (bind ?answer (lowcase ?answer))))
   ?answer)

; Диагностические правила

; Правило определяющее способ передвижения животного
; по ответам пользователя: летает, ходит или ползает.
; Животное может иметь крылья, лапы или являться змеей
(defrule determine-movement-type ""
   =>
   (bind ?response 
      (ask-question "Zhivoe sushchestvo letaet(1), hodit(2) ili polzaet(3)? " 1 2 3))
   (if (eq ?response 1) 
      then 
         (assert (movement-type krylya))
      else 
         (if (eq ?response 2) 
            then 
               (assert (movement-type lapy))
            else 
               (assert (animal "zmeya.")))))

; Правило определяющее животное по его типу крыльев, если оно их
; имеет, по ответам пользователя: с оперением или перепончатые.
; Животное может быть птицой или летучей мышью
(defrule determine-wing-type ""
   (movement-type krylya)
   (not (animal ?))
   =>
   (bind ?response 
      (ask-question "Kakie krylya: imeyut operen'e(4) ili pereponchatye(5)? " 4 5))
      (if (eq ?response 4)
        then 
	  (assert (animal "ptitsa."))
      else 
        (assert (animal "letuchaya mysh."))))

; Правило определяющее животное травоядное или хищное 
; по ответам пользователя, если оно имеет лапы.
(defrule determine-diet-type
   (movement-type lapy)
   (not (animal ?))
   =>
   (bind ?response 
      (ask-question "Zhivotnoe hishchnoe(6) ili travoyadnoe(7)? " 6 7))
      (if (eq ?response 6)
        then 
	  (assert (diet-type hishchnik))
      else 
        (assert (diet-type travoyadnoe))))

; Правило определяющее животное по его окрасу, если оно 
; хищное, по ответам пользователя: полосатое или
; пятнистое. Животное может быть тигром или леопардом
(defrule determine-coat-pattern
   (diet-type hishchnik)
   (not (animal ?))
   =>
   (bind ?response 
      (ask-question "Kakoj okras u zhivotnogo: polosatyj(8) ili pyatnistyj(9)? " 8 9))
      (if (eq ?response 8)
        then 
	  (assert (animal "tigr."))
      else 
        (assert (animal "leopard."))))

; Правило определяющее животное по наличию хобота или
; рогов, если оно травоядное, по ответам пользователя.
; Животное может быть слоном или оленем
(defrule determine-special-features
   (diet-type travoyadnoe)
   (not (animal ?))
   =>
   (bind ?response 
      (ask-question "U zhivotnogo hobot(10) ili roga(11)? " 10 11))
      (if (eq ?response 10)
        then 
	  (assert (animal "slon."))
      else 
        (assert (animal "olen."))))

; Запуск и завершение

; Правило выводит на экран название экспертной
; системы при каждом новом запуске
(defrule system-banner ""
  (declare (salience 10))
  =>
  (printout t crlf crlf)
  (printout t "Expert animal identification system")
  (printout t crlf crlf))

; Правило выводит на экран сообщение
; по определению животного
(defrule print-suggest ""
  (declare (salience 10))
  (animal ?item)
  =>
  (printout t crlf crlf)
  (printout t "Eto zhivotnoe: ")
  (format t " %s%n%n%n" ?item))

