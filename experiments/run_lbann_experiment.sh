#!/bin/bash

# Experiment parameters
EXPERIMENT_NAME=lbann_alexnet
LBANN_DIR=$(git rev-parse --show-toplevel)
READER_PROTO="--reader=${LBANN_DIR}/model_zoo/prototext/data_reader_imagenet.prototext"
MODEL_PROTO="--model=${LBANN_DIR}/model_zoo/prototext/model_alexnet.prototext --num_epochs=10"
OPTIMIZER_PROTO="--optimizer=${LBANN_DIR}/model_zoo/prototext/opt_adam.prototext"
DATASET_CLASSES=10 # default: 1000 (options: 10, 100, 300, 1000)

# Hardware configuration
CLUSTER=
PARTITION=
ACCOUNT=
NUM_NODES= # default: number of allocated nodes (1 if none)
PROCS_PER_NODE= # default: 2 (1 if NUM_NODES=1)
REBOOT_NODES= # default: NO
USE_GPU=
TIME_LIMIT= # default: 12:00:00

# Additional parameters
HOME_DIR=${LBANN_DIR}/experiments
EXPERIMENT_SCRIPT=$(readlink -f "$0")
SUBMIT_JOB= # default: YES
CACHE_DATASET=YES # default: NO
CACHE_DIR=
LUSTRE_DIR=/p/lscratchf/brainusr
DATASET_DIR=datasets/ILSVRC2012
USE_VTUNE= # default: NO
VTUNE_EXE=amplxe-cl-mpi
VTUNE_ARGS="-collect hotspots "

# Set defaults
EXPERIMENT_NAME=${EXPERIMENT_NAME:-lbann}
DATASET_CLASSES=${DATASET_CLASSES:-1000}
if [ -z "${NUM_NODES}" ]; then
    if [ -n "${SLURM_JOB_NUM_NODES}" ]; then
        NUM_NODES=${SLURM_JOB_NUM_NODES}
    else
        NUM_NODES=1
    fi
fi
if [ -z "${PROCS_PER_NODE}" ]; then
    if [ "${NUM_NODES}" -eq 1 ]; then
        PROCS_PER_NODE=1
    else
        PROCS_PER_NODE=2
    fi
fi
REBOOT_NODES=${REBOOT_NODES:-NO}
TIME_LIMIT=${TIME_LIMIT:-12:00:00}
SUBMIT_JOB=${SUBMIT_JOB:-YES}
CACHE_DATASET=${CACHE_DATASET:-NO}
USE_VTUNE=${USE_VTUNE:-NO}

# Set cluster-specific defaults
CLUSTER=${CLUSTER:-$(hostname | sed 's/\([a-zA-Z][a-zA-Z]*\)[0-9]*/\1/g')}
case ${CLUSTER} in
    "catalyst")
        ACCOUNT=${ACCOUNT:-brain}
        PARTITION=${PARTITION:-pbatch}
        USE_GPU=${USE_GPU:-NO}
        CACHE_DIR=${CACHE_DIR:-/l/ssd/${USER}}
        ;;
    "quartz")
        ACCOUNT=${ACCOUNT:-brain}
        PARTITION=${PARTITION:-pbatch}
        USE_GPU=${USE_GPU:-NO}
        CACHE_DIR=${CACHE_DIR:-/tmp/${USER}}
        ;;
    "surface")
        ACCOUNT=${ACCOUNT:-hpclearn}
        PARTITION=${PARTITION:-gpgpu}
        USE_GPU=${USE_GPU:-YES}
        CACHE_DIR=${CACHE_DIR:-/tmp/${USER}}
        ;;
    *)
        echo "Error: unrecognized system (${CLUSTER})"
        exit 1
        ;;
esac

# Initialize dataset
case ${DATASET_CLASSES} in
    "10")
        DATASET_SUFFIX=_c0-9
        ;;
    "100")
        DATASET_SUFFIX=_c0-99
        ;;
    "300")
        DATASET_SUFFIX=_c0-299
        ;;
    *)
        DATASET_SUFFIX=
        ;;
esac
case ${CACHE_DATASET} in
    YES|yes|TRUE|true|ON|on|1)
        ;;
    *)
        CACHE_DIR=${LUSTRE_DIR}
        ;;
esac
READER_PROTO="${READER_PROTO} --data_filedir_train=${CACHE_DIR}/${DATASET_DIR}/resized_256x256/train/ --data_filename_train=${CACHE_DIR}/${DATASET_DIR}/labels/train${DATASET_SUFFIX}.txt --data_filedir_test=${CACHE_DIR}/${DATASET_DIR}/resized_256x256/val/ --data_filename_test=${CACHE_DIR}/${DATASET_DIR}/labels/val${DATASET_SUFFIX}.txt"

# Initialize experiment command
LBANN_EXE="${LBANN_DIR}/build/${CLUSTER}.llnl.gov/model_zoo/lbann"
EXPERIMENT_COMMAND="${LBANN_EXE} ${MODEL_PROTO} ${OPTIMIZER_PROTO} ${READER_PROTO}"

# Make directories
EXPERIMENT_DIR=${HOME_DIR}/$(date +%Y%m%d_%H%M%S)_${EXPERIMENT_NAME}_${CLUSTER}_N${NUM_NODES}
mkdir -p ${EXPERIMENT_DIR}
case ${USE_VTUNE} in
    YES|yes|TRUE|true|ON|on|1)
        VTUNE_DIR=${EXPERIMENT_DIR}/vtune
        mkdir ${VTUNE_DIR}
        ;;
esac

# Move to experiment directory
pushd ${EXPERIMENT_DIR}

# Copy experiment script to directory
cp ${EXPERIMENT_SCRIPT} ${EXPERIMENT_DIR}

# Output parameters and set Slurm settings
SLURM_SCRIPT=${EXPERIMENT_DIR}/slurm_script.sh
LOG_FILE=${EXPERIMENT_DIR}/output.txt
echo "#!/bin/bash"                                       > ${SLURM_SCRIPT}
echo ""                                                 >> ${SLURM_SCRIPT}
echo "# ======== Experiment parameters ========"        >> ${SLURM_SCRIPT}
echo "# Slurm script generated by ${EXPERIMENT_SCRIPT}" >> ${SLURM_SCRIPT}
echo "# Directory: ${EXPERIMENT_DIR}"                   >> ${SLURM_SCRIPT}
echo "# Time: $(date "+%Y-%m-%d %H:%M:%S")"             >> ${SLURM_SCRIPT}
echo "# EXPERIMENT_NAME: ${EXPERIMENT_NAME}"            >> ${SLURM_SCRIPT}
echo "# LBANN_DIR: ${LBANN_DIR}"                        >> ${SLURM_SCRIPT}
echo "# EXPERIMENT_COMMAND: ${EXPERIMENT_COMMAND}"      >> ${SLURM_SCRIPT}
echo "# CLUSTER: ${CLUSTER}"                            >> ${SLURM_SCRIPT}
echo "# PARTITION: ${PARTITION}"                        >> ${SLURM_SCRIPT}
echo "# ACCOUNT: ${ACCOUNT}"                            >> ${SLURM_SCRIPT}
echo "# NUM_NODES: ${NUM_NODES}"                        >> ${SLURM_SCRIPT}
echo "# PROCS_PER_NODE: ${PROCS_PER_NODE}"              >> ${SLURM_SCRIPT}
echo "# REBOOT_NODES: ${REBOOT_NODES}"                  >> ${SLURM_SCRIPT}
echo "# USE_GPU: ${USE_GPU}"                            >> ${SLURM_SCRIPT}
echo "# TIME_LIMIT: ${TIME_LIMIT}"                      >> ${SLURM_SCRIPT}
echo "# SUBMIT_JOB: ${SUBMIT_JOB}"                      >> ${SLURM_SCRIPT}
echo "# CACHE_DATASET: ${CACHE_DATASET}"                >> ${SLURM_SCRIPT}
echo "# CACHE_DIR: ${CACHE_DIR}"                        >> ${SLURM_SCRIPT}
echo "# LUSTRE_DIR: ${LUSTRE_DIR}"                      >> ${SLURM_SCRIPT}
echo "# DATASET_DIR: ${DATASET_DIR}"                    >> ${SLURM_SCRIPT}
echo "# USE_VTUNE: ${USE_VTUNE}"                        >> ${SLURM_SCRIPT}
echo ""                                                 >> ${SLURM_SCRIPT}
echo "# ======== Slurm settings ========"               >> ${SLURM_SCRIPT}
echo "#SBATCH --account=${ACCOUNT}"                     >> ${SLURM_SCRIPT}
echo "#SBATCH --workdir=${EXPERIMENT_DIR}"              >> ${SLURM_SCRIPT}
echo "#SBATCH --error=${LOG_FILE}"                      >> ${SLURM_SCRIPT}
echo "#SBATCH --job-name=${EXPERIMENT_NAME}"            >> ${SLURM_SCRIPT}
echo "#SBATCH --nodes=${NUM_NODES}"                     >> ${SLURM_SCRIPT}
echo "#SBATCH --output=${LOG_FILE}"                     >> ${SLURM_SCRIPT}
echo "#SBATCH --partition=${PARTITION}"                 >> ${SLURM_SCRIPT}
case ${REBOOT_NODES} in
    YES|yes|TRUE|true|ON|on|1)
        echo "#SBATCH --reboot"                         >> ${SLURM_SCRIPT}
esac
echo "#SBATCH --time=${TIME_LIMIT}"                     >> ${SLURM_SCRIPT}
echo ""                                                 >> ${SLURM_SCRIPT}
echo "# ======== Print node name ========"              >> ${SLURM_SCRIPT}
SRUN_COMMAND="srun --nodes=${NUM_NODES} --ntasks-per-node=1"
echo "${SRUN_COMMAND} hostname"                         >> ${SLURM_SCRIPT}
echo ""                                                 >> ${SLURM_SCRIPT}

# Cache dataset in node-local memory
case ${CACHE_DATASET} in
    YES|yes|TRUE|true|ON|on|1)
        echo "# ======== Cache dataset ========" >> ${SLURM_SCRIPT}
        echo "echo \"Caching dataset...\"" >> ${SLURM_SCRIPT}
        BCAST="/collab/usr/global/tools/stat/file_bcast/${SYS_TYPE}/fbcast/file_bcast_par13 1MB"
        SRUN_COMMAND="srun --nodes=${NUM_NODES} --ntasks-per-node=1"
        COPY_COMMAND="srun --nodes=${NUM_NODES} --ntasks-per-node=2 ${BCAST}"
        echo "${SRUN_COMMAND} mkdir -p ${CACHE_DIR}/${DATASET_DIR}/resized_256x256" >> ${SLURM_SCRIPT}
        echo "${SRUN_COMMAND} mkdir -p ${CACHE_DIR}/${DATASET_DIR}/labels" >> ${SLURM_SCRIPT}
        for TARBALL in resized_256x256/train.tar resized_256x256/val.tar
        do
            LUSTRE_FILE=${LUSTRE_DIR}/${DATASET_DIR}/${TARBALL}
            CACHE_FILE=${CACHE_DIR}/${DATASET_DIR}/$(basename ${TARBALL})
            OUTPUT_DIR=${CACHE_DIR}/${DATASET_DIR}/resized_256x256/$(basename ${TARBALL} .tar)
            echo "[ -e ${CACHE_FILE} ] || \\" >> ${SLURM_SCRIPT}
            echo "  ${COPY_COMMAND} ${LUSTRE_FILE} ${CACHE_FILE} > /dev/null" >> ${SLURM_SCRIPT}
            echo "[ -d ${OUTPUT_DIR} ] || \\" >> ${SLURM_SCRIPT}
            echo "  ${SRUN_COMMAND} tar xf ${CACHE_FILE} -C ${CACHE_DIR}/${DATASET_DIR}/resized_256x256" >> ${SLURM_SCRIPT}
        done
        for TARBALL in labels.tar
        do
            LUSTRE_FILE=${LUSTRE_DIR}/${DATASET_DIR}/${TARBALL}
            CACHE_FILE=${CACHE_DIR}/${DATASET_DIR}/$(basename ${TARBALL})
            OUTPUT_FILE=${CACHE_DIR}/${DATASET_DIR}/labels/train.txt
            echo "[ -e ${CACHE_FILE} ] || \\" >> ${SLURM_SCRIPT}
            echo "  ${COPY_COMMAND} ${LUSTRE_FILE} ${CACHE_FILE} > /dev/null" >> ${SLURM_SCRIPT}
            echo "[ -e ${OUTPUT_FILE} ] || \\" >> ${SLURM_SCRIPT}
            echo "  ${SRUN_COMMAND} tar xf ${CACHE_FILE} -C ${CACHE_DIR}/${DATASET_DIR}" >> ${SLURM_SCRIPT}
        done
        echo "wait" >> ${SLURM_SCRIPT}
        echo "echo \"Done caching dataset...\"" >> ${SLURM_SCRIPT}
        echo "" >> ${SLURM_SCRIPT}
        ;;
esac

# Set experiment
echo "# ======== Experiment ========" >> ${SLURM_SCRIPT}
SRUN_COMMAND="srun --nodes=${NUM_NODES} --ntasks-per-node=${PROCS_PER_NODE}"
case ${USE_GPU} in
    YES|yes|TRUE|true|ON|on|1)
        SRUN_COMMAND="${SRUN_COMMAND} --nvidia_compute_mode=default"
        ;;
esac
case ${USE_VTUNE} in
    YES|yes|TRUE|true|ON|on|1)
        VTUNE_COMMAND="${VTUNE_EXE} -r ${VTUNE_DIR} -app-working-dir ${EXPERIMENT_DIR} ${VTUNE_ARGS} --"
        ;;
esac
echo "${SRUN_COMMAND} ${VTUNE_COMMAND} ${EXPERIMENT_COMMAND}" >> ${SLURM_SCRIPT}

# Submit script to Slurm
case ${SUBMIT_JOB} in
    YES|yes|TRUE|true|ON|on|1)
        if [ -z "${SLURM_JOB_ID}" ]; then
            sbatch ${SLURM_SCRIPT} > ${LOG_FILE} 2>&1
        else
            sh ${SLURM_SCRIPT} > ${LOG_FILE} 2>&1
        fi
        ;;
esac

# Return to original directory
dirs -c
