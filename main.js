window.addEventListener("load", (event) => {
    const targetTag = document.getElementById("target");
    const outputTag = document.getElementById("output");
    const formTag = document.getElementById("form");
    const fieldsetTag = document.getElementById("fields");
    const onmessage = (event) => {
        const data = event.data;
        outputTag.value = data.success ? data.result : "No solution";
        fieldsetTag.removeAttribute("disabled");
        document.documentElement.style.cursor = "auto";
    };
    let worker = resetWorker(null, "worker.js", onmessage);

    formTag.addEventListener("submit", (event) => {
        event.preventDefault();
        const targetValue = parseInt(targetTag.value);

        if(isNaN(targetValue)) {
            return;
        }

        worker.postMessage(targetValue);
        fieldsetTag.setAttribute("disabled", true);
        document.documentElement.style.cursor = "wait";
    });
    formTag.addEventListener("reset", (event) => {
        worker = resetWorker(worker, "worker.js", onmessage);
        fieldsetTag.removeAttribute("disabled");
        document.documentElement.style.cursor = "auto";
    });
});

function resetWorker(worker, file, onmessage) {
    if(worker != null) {
        worker.terminate();
    }

    worker = new Worker(file);
    worker.addEventListener("message", onmessage);
    return worker;
}
