(ns kostassoid.sandbox.rerenamer
  (:require [clojure.java.io :as io])
  (:gen-class :main true))

(defn find-and-replace [path pattern replacement what-if?]
  (println "path=" path)
  (println "pattern=" pattern)
  (println "replacement=" replacement)
  (println "whatif=" what-if?)
)

(defn show-usage []
  (println "Usage: ReRenamer <path> <pattern> <replacement> [-whatif]" ))

(defn -main
  [& args]
  (if (<= 3 (count args))
    (let[[path pattern replacement what-if?] args]
      (find-and-replace path pattern replacement (= "-whatif" what-if?))
    )
    (show-usage)
  )
)

(apply -main *command-line-args*)

