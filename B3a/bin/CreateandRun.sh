enrich=0.9
r=7.5

for i in {1..50..1}
do
sleep 1
mac=Keff_${enrich}U${r}cm_${i}.mac
echo "create $mac"
echo "/run/initialize" > $mac
echo "/process/had/particle_hp/use_Wendt_fission_model true" >> $mac
echo "/analysis/setDefaultFileType root" >> $mac
echo "/analysis/openFile neutron_${i}" >> $mac
echo "/gun/particle neutron" >> $mac
echo "/gun/energy 0.025 eV" >> $mac
echo "/run/beamOn 3" >> $mac
echo "/analysis/write" >> $mac
echo "/analysis/closeFile" >> $mac
#echo "/analysis/reset" >> $mac

./exampleB3a $mac #1>log${enrich}U${r}cm${n}EvtSn${i}.txt 2>logE${enrich}U${r}cm${n}EvtSn${i}.txt&
done

hadd total.root neutron_*.root