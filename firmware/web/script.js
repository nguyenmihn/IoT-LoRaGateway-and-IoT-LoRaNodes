const firebaseConfig = {
  apiKey: "AIzaSyAwJhUJPq8iOFzXZHktYPjynFZR8hDQ5WU",
  authDomain: "lora-c0c0b.firebaseapp.com",
  databaseURL: "https://lora-c0c0b-default-rtdb.firebaseio.com",
  projectId: "lora-c0c0b",
  storageBucket: "lora-c0c0b.firebasestorage.app",
  messagingSenderId: "289002166937",
  appId: "1:289002166937:web:422209d0d194904f00f608"
};
firebase.initializeApp(firebaseConfig);

// Âm thanh cảnh báo
const alarmSound = new Audio("baochay1.mp3");
alarmSound.preload = "auto";
alarmSound.volume = 1.0;

// Kích hoạt âm thanh (phải có click đầu tiên)
document.addEventListener("click", () => {
  alarmSound.play().catch(() => {});
}, { once: true });

document.addEventListener("DOMContentLoaded", () => {
  document.querySelectorAll(".rack").forEach(rack => {
    const id = rack.dataset.id;
    if (!id || rack.classList.contains("add-new")) return;
    initRack(rack, id);
  });

  attachDoubleClickDelete(); // Gắn sự kiện dblclick
});


function showPopup(id) {
  const popup = document.getElementById("popup");
  const title = document.getElementById("popup-id");
  const content = document.getElementById("popup-content");

  title.textContent = `Chi tiết cảm biến: ${id}`;
  content.textContent = "Đang tải...";

  firebase.database().ref(`LoRa/${id}`).once("value").then((snap) => {
    const data = snap.val();
    if (data) {
      content.innerHTML = `
        🌡️ Nhiệt độ môi trường: ${data.Ambient} ℃<br>
        🔥 Cảm biến lửa: ${data.Flame}<br>
        💨 Nồng độ khí: ${data.Gas}<br>
        ⚙️ MAX6675: ${data.MAX6675}<br>
        📍 Nhiệt độ vật thể: ${data.Object} ℃
      `;
    } else {
      content.textContent = "Không có dữ liệu.";
    }
  });

  popup.classList.add("visible");
}

function hidePopup() {
  document.getElementById("popup").classList.remove("visible");
}

function showAddForm() {
  document.getElementById("addForm").classList.add("visible");
}

function hideAddForm() {
  document.getElementById("addForm").classList.remove("visible");
}

function addNewRack() {
  const id = document.getElementById("newSensorId").value.trim();

  if (!id) return alert("⚠️ Vui lòng nhập ID cảm biến!");
  const existing = Array.from(document.querySelectorAll(".rack"))
    .some(r => r.dataset.id === id);
  if (existing) {
    return alert("❗ ID đã tồn tại trong hệ thống.");
  }

  const panel = document.querySelector(".panel-grid");
  const addNewBtn = document.querySelector(".rack.add-new");

  const currentRacks = document.querySelectorAll(".rack:not(.add-new)");
  const newDisplayId = String(currentRacks.length + 1).padStart(2, "0");

  const rack = document.createElement("div");
  rack.className = "rack";
  rack.dataset.id = id;
  rack.dataset.displayId = newDisplayId;

  panel.insertBefore(rack, addNewBtn);
  initRack(rack, id);

  alert("✅ Đã thêm tủ điện mới thành công.");
  hideAddForm();
  document.getElementById("newSensorId").value = "";
}

// ✅ Hàm xác nhận xóa
function confirmRemove(event, id, rackElement) {
  event.stopPropagation(); // Không mở popup
  const confirmDelete = confirm(`❗ Bạn có chắc muốn xóa cảm biến với MAC: ${id} không?`);
  if (confirmDelete) {
    rackElement.remove();
    alert("✅ Đã xóa tủ điện khỏi giao diện.");
  }
}

// ✅ Hàm khởi tạo rack
function initRack(rack, id) {
  if (!id || id === "undefined") {
    console.warn("ID không hợp lệ:", id);
    rack.remove();
    return;
  }

  const displayId = rack.dataset.displayId || "??";
  rack.innerHTML = `
    <div class="close-btn" onclick="confirmRemove(event, '${id}', this.parentElement)">×</div>
    <img src="screenshot_1748689128.png" />
    <div class="sensor-id">ID: ${displayId}<br>MAC: ${id}</div>
    <div class="details">
      <div>🌡️ <span class="ambient">--</span> ℃</div>
      <div>🔥 <span class="flame">--</span></div>
      <div>💨 <span class="gas">--</span></div>
      <div>⚙️ <span class="max6675">--</span></div>
      <div>📍 <span class="object">--</span> ℃</div>
    </div>
  `;

  rack.addEventListener("click", () => showPopup(id));

  const ambientEl = rack.querySelector(".ambient");
  const flameEl = rack.querySelector(".flame");
  const gasEl = rack.querySelector(".gas");
  const maxEl = rack.querySelector(".max6675");
  const objectEl = rack.querySelector(".object");

  firebase.database().ref(`LoRa/${id}`).on('value', (snap) => {
    const data = snap.val();
    if (!data) return;

    ambientEl.textContent = data.Ambient ?? "--";
    flameEl.textContent = data.Flame ?? "--";
    gasEl.textContent = data.Gas ?? "--";
    maxEl.textContent = data.MAX6675 ?? "--";
    objectEl.textContent = data.Object ?? "--";

    rack.classList.remove("overheat", "flame-alert");

    if (data.Object > 50) {
      rack.classList.add("overheat");
      alarmSound.cloneNode(true).play().catch(() => {});
    }

    if (data.Flame > 1000) {
      rack.classList.add("flame-alert");
      alarmSound.cloneNode(true).play().catch(() => {});
    }
  });
}

function switchFloor(floor) {
  document.querySelectorAll(".panel-grid").forEach(grid => {
    grid.style.display = grid.dataset.floor == floor ? "flex" : "none";
  });
  document.querySelectorAll(".floor-btn").forEach(btn => btn.classList.remove("active"));
  document.querySelectorAll(".floor-btn").forEach(btn => {
    if (btn.textContent === `Lầu ${floor}`) btn.classList.add("active");
  });
}


function addNewFloor() {
  const floorContainer = document.getElementById("floor-panels");
  const floorButtons = document.getElementById("floor-buttons");

  const currentFloors = floorContainer.querySelectorAll(".panel-grid").length;
  const newFloorNum = currentFloors + 1;

  const newBtn = document.createElement("button");
  newBtn.className = "floor-btn";
  newBtn.textContent = `Lầu ${newFloorNum}`;
  newBtn.onclick = () => switchFloor(newFloorNum);
  floorButtons.insertBefore(newBtn, floorButtons.querySelector(".add-floor-btn"));

  const newPanel = document.createElement("div");
  newPanel.className = "panel-grid";
  newPanel.dataset.floor = newFloorNum;
  newPanel.style.display = "none";
  newPanel.innerHTML = `
    <div class="rack add-new" onclick="showAddForm()">
      <div class="plus">+</div>
      <div class="label">Thêm tủ mới</div>
    </div>
  `;
  floorContainer.appendChild(newPanel);

  switchFloor(newFloorNum);
}
function showFloorNameForm() {
  document.getElementById("floorNameForm").style.display = "block";
}
function hideFloorNameForm() {
  document.getElementById("floorNameForm").style.display = "none";
}
function switchFloor(floor) {
  document.querySelectorAll('.panel-grid').forEach(el => {
    el.style.display = el.getAttribute('data-floor') === floor ? 'flex' : 'none';
  });
  document.querySelectorAll('.floor-btn').forEach(btn => {
    btn.classList.toggle('active', btn.textContent.trim() === floor || btn.textContent.trim() === `Lầu ${floor}`);
  });
}
function confirmAddNewFloor() {
  const nameInput = document.getElementById("newFloorName");
  const floorName = nameInput.value.trim();
  if (!floorName) return alert("Vui lòng nhập tên lầu.");

  const floorId = Date.now().toString();

  const floorBtn = document.createElement("button");
  floorBtn.className = "floor-btn";
  floorBtn.textContent = floorName;
  floorBtn.onclick = () => switchFloor(floorId);
  floorBtn.ondblclick = () => {
    if (confirm(`Bạn có chắc muốn xóa ${floorName}?`)) {
      floorBtn.remove();
      const panel = document.querySelector(`.panel-grid[data-floor='${floorId}']`);
      if (panel) panel.remove();
    }
  };

  const buttonContainer = document.getElementById("floor-buttons");
  const addBtn = buttonContainer.querySelector(".add-floor-btn");
  buttonContainer.insertBefore(floorBtn, addBtn);

  const panel = document.createElement("div");
  panel.className = "panel-grid";
  panel.setAttribute("data-floor", floorId);
  panel.style.display = "none";
  panel.innerHTML = `
    <div class="rack add-new" onclick="showAddForm()">
      <div class="plus">+</div>
      <div class="label">Thêm tủ mới</div>
    </div>
  `;
  document.getElementById("floor-panels").appendChild(panel);

  hideFloorNameForm();
  switchFloor(floorId);
  nameInput.value = "";
}
// Gắn sự kiện double click xóa cho tất cả các nút Lầu, kể cả cứng
function attachDoubleClickDelete() {
  document.querySelectorAll(".floor-btn").forEach(btn => {
    if (btn.classList.contains("add-floor-btn")) return;

    btn.ondblclick = () => {
      const name = btn.textContent.trim();
      if (confirm(`❗ Bạn có chắc muốn xóa ${name}?`)) {
        const floor = name.replace("Lầu ", "").trim();
        const panel = document.querySelector(`.panel-grid[data-floor='${floor}']`);
        if (panel) panel.remove();
        btn.remove();
      }
    };
  });
}
